from conans import ConanFile, CMake

class VisvidConan(ConanFile):
    requires = [
        "ffmpeg/4.2@bincrafters/stable",
        "sdl2/2.0.10@bincrafters/stable"
    ]
    generators = ["cmake_paths"]
    default_options = {"ffmpeg:shared": True}

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin