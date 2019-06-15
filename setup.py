import os
import platform
import shutil

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.build_clib import build_clib
from distutils.cmd import Command
from urllib import request
import tarfile

PYBIND11_URL = "https://github.com/pybind/pybind11/archive/v2.2.3.tar.gz"


class PackageSharedSource(Command):
    description = "include shared source files"
    user_options = [
        ("shared-source-path",
         None,
         "Location of shared source files to package")
    ]

    def initialize_options(self):

        self.shared_source_path = os.path.abspath(
            os.path.join("examples", "shared"))

    def finalize_options(self):
        pass

    def run(self):
        output_dest = os.path.abspath(
            os.path.join("shared")
        )

        if os.path.exists(output_dest):
            self.announce("{} already exists".format(output_dest), level=5)
            return

        self.announce(f"Copying shared directory "
                      f"{self.shared_source_path} to source tree",
                      level=5)
        shutil.copytree(self.shared_source_path, output_dest)


class PackageVisvid(Command):
    description = "Create Source archive of Visvid for packaging"
    user_options = []

    def initialize_options(self):
        self.visvis_source = os.path.abspath(".")

    def finalize_options(self):
        pass

    def run(self):
        self.make_visvid_source()

    def make_visvid_source(self):
        clib_cmd = self.get_finalized_command("build_clib")
        cmake_path = clib_cmd.cmake_path

        config_path = os.path.abspath(
            os.path.join(clib_cmd.build_temp,
                         "cmake_config")
        )

        self.mkpath(config_path)
        cmake_config_command = [
            cmake_path,
            "-S", self.visvis_source,
            "-B", config_path,
            "-DCPACK_SOURCE_PACKAGE_FILE_NAME=visvis_source"
        ]

        self.spawn(cmake_config_command)

        source_config_file = os.path.join(config_path,
                                          "CPackSourceConfig.cmake")

        assert os.path.exists(source_config_file), \
            "Required file, {}, was not generated".format(source_config_file)

        cmake_cpack_command = [
            "cpack",
            "--config", source_config_file,
            "-G", "TGZ"

        ]
        self.spawn(cmake_cpack_command)


class BuildExt(build_ext):

    def run(self):
        self.get_deps()
        clib_cmd = self.get_finalized_command("build_clib")

        # Include the paths built by the clib
        self.include_dirs.append(".")

        self.include_dirs.insert(
            0, os.path.join(clib_cmd.build_clib, "include"))

        self.library_dirs.insert(
            0, os.path.join(clib_cmd.build_clib, "lib"))

        super().run()

    def get_deps(self):
        self.mkpath(self.build_temp)

        archive_dest = os.path.abspath(
            os.path.join(self.build_temp, "pybind11.tar.gz"))

        if not os.path.exists(archive_dest):
            self.announce("Downloading pybind11", level=5)
            request.urlretrieve(PYBIND11_URL, filename=archive_dest)
            self.announce("Downloading pybind11 done", level=1)

        with tarfile.open(archive_dest, "r") as tf:
            self.announce("Extracting pybind11")
            for f in tf:
                dest = os.path.join(self.build_temp, f.name)

                if "pybind11.h" in f.name:

                    pybind11_include_path = os.path.abspath(
                        os.path.join(os.path.dirname(dest), ".."))

                    self.announce(
                        f"Adding {pybind11_include_path} to include path")

                    self.include_dirs.append(pybind11_include_path)

                if os.path.exists(dest):
                    continue
                tf.extract(f, self.build_temp)


class BuildCMakeClib(build_clib):
    user_options = [
        ("cmake-path=", None, "Path to CMake executable")
    ]

    def extract_source(self, archive):
        self.announce("Extracting visvid source to {}".format(
            self.build_temp), level=5)

        with tarfile.open(archive, "r") as archive_file:
            for f in archive_file:
                archive_file.extract(f, self.build_temp)

    def initialize_options(self):
        self.cmake_path = shutil.which("cmake")

        super().initialize_options()

    def finalize_options(self):
        super().finalize_options()

        assert self.cmake_path, "CMake command is required to build visvid"

    def build_libraries(self, libraries):
        for lib_name, lib in libraries:
            build_path = os.path.abspath(os.path.join(self.build_temp,
                                                      "build",
                                                      lib_name))

            if not os.path.exists(os.path.join(build_path, "CMakeCache.txt")):
                install_prefix = os.path.abspath(self.build_clib)
                source_dir = lib['CMAKE_SOURCE_DIR']
                self.compiler.spawn([
                    self.cmake_path,
                    "-S", os.path.abspath(source_dir),
                    "-B", build_path,
                    f"-DCMAKE_INSTALL_PREFIX:PATH={install_prefix}"
                    ]
                )
            self.compiler.spawn([
                self.cmake_path, "--build", build_path, "--target", "install"])


pyvisvid_extension = Extension(
    "pyvisvid.visvid",
    sources=[
        'src/applications/pyvisvid/pyvisvid/visvid_extension.cpp',
        'src/applications/pyvisvid/pyvisvid/Visualizer.cpp',
        'examples/shared/decode.c',
        'examples/shared/pgm.c',
         ],
    include_dirs=[
        "src/applications/pyvisvid/pyvisvid",
        "examples",
    ],
    libraries=["Visvid", "avformat", "avcodec"],
    language='c++'
    )

# On a mac, search homebrew directories for ffmpeg
if platform.system() == "Darwin":
    pyvisvid_extension.include_dirs.append("/usr/local/include")
    pyvisvid_extension.library_dirs.append("/usr/local/lib")

libvisvid = \
    ("Visvid", {
        "sources": [],
        "CMAKE_SOURCE_DIR": os.path.dirname(__file__),
    }
     )


setup(
    name="pyvisvid",
    version="0.1.0",
    author="Henry Borchers",
    author_email="fake@fake.com",
    url="http://www.sample.com",
    packages=["pyvisvid"],
    package_dir={"": "src/applications/pyvisvid"},
    setup_requires=["pytest-runner"],
    test_suite='examples/pyvisvid/tests',
    cmdclass={
        'build_ext': BuildExt,
        "build_clib": BuildCMakeClib,
    },
    libraries=[libvisvid],
    ext_modules=[pyvisvid_extension],
    zip_safe=False,
)
