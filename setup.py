import abc

import os
import platform
import shutil

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.build_clib import build_clib
from distutils.cmd import Command
import tarfile


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
        self.include_dirs.insert(
            0, os.path.join(self.build_temp, "include", "visvid"))
        clib_cmd = self.get_finalized_command("build_clib")
        # Include the paths built by the clib
        self.include_dirs.append("./src/visvid/include")

        self.include_dirs.insert(
            0, os.path.join(clib_cmd.build_clib, "include"))

        self.library_dirs.insert(
            0, os.path.join(clib_cmd.build_clib, "lib"))
        import pybind11
        self.include_dirs.insert(0, pybind11.get_include())
        super().run()


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
        self.cmake_path = None

        super().initialize_options()

    def finalize_options(self):
        import cmake
        if self.cmake_path is None:
            self.cmake_path = shutil.which("cmake", path=cmake.CMAKE_BIN_DIR)

        super().finalize_options()

        assert self.cmake_path, "CMake command is required to build visvid"

    def build_libraries(self, libraries):
        build_ext_cmd = self.get_finalized_command("build_ext")
        for lib_name, lib in libraries:
            if len(lib['sources']) > 0:
                new_lib = lib.copy()
                classic_library = (lib_name, new_lib)

                # Add the same include directories used by pyvisvid.visvid
                include_dirs = lib.get('include_dirs', [])
                for include_dir in build_ext_cmd.include_dirs:
                    include_dirs.append(include_dir)
                lib['include_dirs'] = include_dirs

                super().build_libraries([classic_library])
            else:
                build_path = os.path.abspath(os.path.join(self.build_temp,
                                                          "build",
                                                          lib_name))

                if build_ext_cmd.force == 1 or \
                        not os.path.exists(os.path.join(build_path,
                                                        "CMakeCache.txt")
                                           ):

                    install_prefix = os.path.abspath(self.build_clib)
                    source_dir = lib['CMAKE_SOURCE_DIR']
                    cmake_config_command = [
                        self.cmake_path,
                        "-S", os.path.abspath(source_dir),
                        "-B", build_path,
                        f"-DCMAKE_INSTALL_PREFIX:PATH={install_prefix}",
                        "-DBUILD_TESTING:BOOL=false"
                    ]
                    build_ext_cmd.include_dirs.append(os.path.join(self.build_clib, "include", "visvid"))
                    compiler_flags = list()

                    for flag in self.compiler.compiler:
                        compiler_commands = [
                            "clang",
                            "gcc",
                            "cc",
                            "x86_64-linux-gnu-gcc",
                            self.compiler.compiler[0],
                            self.compiler.compiler_cxx[0]
                        ]

                        # remove the command for the compiler
                        if flag in compiler_commands:
                            continue

                        compiler_flags.append(flag)

                    CMAKE_SHARED_LINKER_FLAGS = " ".join(compiler_flags)

                    cmake_config_command.append(
                        "-DCMAKE_C_FLAGS={}".format(CMAKE_SHARED_LINKER_FLAGS)
                    )

                    self.compiler.spawn(cmake_config_command)

                self.compiler.spawn(
                    [
                        self.cmake_path,
                        "--build", build_path,
                        "--target", "install"
                    ]
                )


pyvisvid_extension = Extension(
    "pyvisvid.visvid",
    sources=[
        'src/applications/pyvisvid/pyvisvid/visvid_extension.cpp',
        'src/applications/pyvisvid/pyvisvid/Visualizer.cpp',
        'src/applications/pyvisvid/pyvisvid/utils.cpp',
        'src/applications/pyvisvid/pyvisvid/generate.cpp',
         ],
    include_dirs=[
        "src/applications/pyvisvid/pyvisvid",
        "examples",
    ],
    libraries=['Visvid', "avformat", "avcodec", "shared_data"],
    language='c++',
    )


class PlatformSpecificRequirements(abc.ABC):
    def add_include_dirs(self):
        """Extra includes"""

    def add_library_dirs(self):
        """Add extra library dirs"""

    def extra_compile_args(self):
        """Add exta Compiler flags"""


class DarwinExtras(PlatformSpecificRequirements):

    def add_include_dirs(self):
        # On a mac, search homebrew directories for ffmpeg
        pyvisvid_extension.include_dirs.append("/usr/local/include")

    def add_library_dirs(self):
        pyvisvid_extension.library_dirs.append("/usr/local/lib")

    def extra_compile_args(self):
        pyvisvid_extension.extra_compile_args.append("-std=c++11")


EXTRA_COMPILER_SETTINGS = {
    "Darwin": DarwinExtras,
}

extra_step_setter = EXTRA_COMPILER_SETTINGS.get(platform.system())
if extra_step_setter is not None:
    extra = extra_step_setter()
    extra.add_include_dirs()
    extra.add_library_dirs()
    extra.extra_compile_args()

libvisvid = \
    ("Visvid", {
        "sources": [],
        "CMAKE_SOURCE_DIR": os.path.dirname(__file__),
    }
     )

share_data = (
    'shared_data',
    {
        'sources': [
            'examples/shared/decode.c',
            'examples/shared/pgm.c',
        ],
        'include_dirs': ["./src/visvid/include"],
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
    package_data={"pyvisvid": ["visvid.pyi"]},
    setup_requires=[
        "pytest-runner",
        "pybind11",
        "cmake"
    ],
    test_suite='examples/pyvisvid/tests',
    cmdclass={
        'build_ext': BuildExt,
        "build_clib": BuildCMakeClib,
    },
    libraries=[libvisvid, share_data],
    ext_modules=[pyvisvid_extension],
    zip_safe=False,
)
