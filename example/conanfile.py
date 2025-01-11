from conan import ConanFile
from conan.tools.cmake import cmake_layout


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"
    requires = ["fmt/10.2.1", "range-v3/0.12.0"]

    def layout(self):
        cmake_layout(self)
