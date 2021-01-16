import os

from conans import ConanFile, CMake, tools


class VisvidConan(ConanFile):
    requires = [
        # "ffmpeg/4.2.1@bincrafters/stable",
        # "libx264/20191217"
    ]
    settings = "os", "arch", "compiler", "build_type"

    generators = ["cmake_paths"]
    options = {
        "with_createVisuals": [True, False]
    }
    default_options = {
        "with_createVisuals": False,
        # "ffmpeg:mp3lame": False,
        # "ffmpeg:vpx": False,
        # "ffmpeg:openssl": False,
        # "ffmpeg:webp": False,
    }

    def requirements(self):
        if self.settings.os == "Windows":
            self.requires("ffmpeg/4.2.1@bincrafters/stable")
        #     Rely on system package management for linux and mac

        if self.options.with_createVisuals:
            self.requires("sdl2/2.0.12@bincrafters/stable")
            if self.settings.os == "Linux":
                self.requires("libalsa/1.2.4")


    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

    def system_requirements(self):
        if self.settings.os == "Macos" and tools.os_info.is_macos:
            package_tool = tools.SystemPackageTool()
            packages = []
            packages.append("ffmpeg")
            for package in packages:
                package_tool.install(package)
            # if self.settings.os == "Linux" or self.settings.os == "Macos":
            #     self.options["ffmpeg"].vorbis = False
            #     self.options["ffmpeg"].openjpeg = False
            #     self.options["ffmpeg"].x264 = False
            #     self.options["ffmpeg"].x265 = False

            # if self.settings.os == "Macos":
            #     self.options["ffmpeg"].audiotoolbox = False
            #     self.options["ffmpeg"].openh264 = False
            #     # self.options["ffmpeg"].opus = False
            #     self.options["ffmpeg"].videotoolbox = False
            #     self.options["ffmpeg"].iconv = False
            #     self.options["ffmpeg"].bzlib = False
            #     self.options["ffmpeg"].zlib = False
            #     self.options["ffmpeg"].lzma = False
            #     self.options["ffmpeg"].fdk_aac = False

        # super().system_requirements()

    # def configure(self):
    #     if self.settings.os == "Linux" or self.settings.os == "Macos":
    #         self.options["ffmpeg"].vorbis = False
    #         self.options["ffmpeg"].openjpeg = False
    #         self.options["ffmpeg"].x264 = False
    #         self.options["ffmpeg"].x265 = False
    #
    #     if self.settings.os == "Macos":
    #         self.options["ffmpeg"].audiotoolbox = False
    #         self.options["ffmpeg"].openh264 = False
    #         # self.options["ffmpeg"].opus = False
    #         self.options["ffmpeg"].videotoolbox = False
    #         self.options["ffmpeg"].iconv = False
    #         self.options["ffmpeg"].bzlib = False
    #         self.options["ffmpeg"].zlib = False
    #         self.options["ffmpeg"].lzma = False
    #         self.options["ffmpeg"].fdk_aac = False

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
