import os

from conans import ConanFile, CMake

class VisvidConan(ConanFile):
    requires = [
        "ffmpeg/5.0",
    ]
    settings = "os", "arch", "compiler", "build_type"

    generators = ["cmake_paths", "cmake_find_package"]
    options = {
        "with_createVisuals": [True, False]
    }
    default_options = {
        "with_createVisuals": False,
    }

    def requirements(self):
        if self.options.with_createVisuals:
            self.requires("sdl/2.24.0")
            # if self.settings.os == "Linux":
            #     self.requires("libalsa/1.2.4")


    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

    def configure(self):
        if self.settings.os == "Linux":
            # self.options["ffmpeg"].vorbis = False
            self.options["ffmpeg"].with_openjpeg = False
            self.options["ffmpeg"].with_libx264 = False
            self.options["ffmpeg"].with_libx265 = False

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
