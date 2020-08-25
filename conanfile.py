import os

from conans import ConanFile, CMake

class VisvidConan(ConanFile):
    requires = [
        "ffmpeg/4.2.1@bincrafters/stable",
        "sdl2/2.0.12@bincrafters/stable",
        "libiconv/1.16"
    ]
    settings = "os", "arch", "compiler", "build_type"

    generators = ["cmake_paths"]
    default_options = {
        # "ffmpeg:shared": True,
        "ffmpeg:openjpeg": False,
        "ffmpeg:x264": False,
        "ffmpeg:x265": False,
    }

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

    def configure(self):
        if self.settings.os == "Linux":
            self.options["ffmpeg"].vorbis = False

    def build(self):
        cmake = CMake(self)
        cmake_toolchain_file = os.path.join(self.build_folder, "conan_paths.cmake")
        assert os.path.exists(cmake_toolchain_file)
        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = cmake_toolchain_file
        cmake.configure()
        cmake.build()

