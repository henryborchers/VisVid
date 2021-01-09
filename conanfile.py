import os

from conans import ConanFile, CMake

class VisvidConan(ConanFile):
    requires = [
        "ffmpeg/4.2.1@bincrafters/stable",
        "libiconv/1.16"
    ]
    settings = "os", "arch", "compiler", "build_type"

    generators = ["cmake_paths"]
    options = {
        "with_createVisuals": [True, False]
    }
    default_options = {
        "with_createVisuals": False,
    }
    def requirements(self):
        if self.options.with_createVisuals:
            self.requires("sdl2/2.0.12@bincrafters/stable")


    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

    def configure(self):
        if self.settings.os == "Linux":
            self.options["ffmpeg"].vorbis = False

    def build(self):
        cmake = self._configure_cmake()

        cmake.build()

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake_toolchain_file = os.path.join(self.build_folder, "conan_paths.cmake")
        assert os.path.exists(cmake_toolchain_file)
        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = cmake_toolchain_file
        cmake.definitions["VISVID_SAMPLE_CREATEVISUALS"] = self.options.with_createVisuals
        cmake.definitions["BUILD_TESTING"] = False
        cmake.configure()
        return cmake
