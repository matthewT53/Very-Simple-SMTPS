from conan import ConanFile
from conan.tools.meson import MesonToolchain, Meson

class VerySimpleSMTPSConan(ConanFile):
    generators = "PkgConfigDeps"
    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False]}
    default_options = {"shared": False}

    required_packages = [
        "libcurl/8.0.1",
        "cpputest/4.0"
    ]

    def requirements(self):
        for package in self.required_packages:
            self.requires(package)

    def generate(self):
        tc = MesonToolchain(self)
        tc.generate()

    def build(self):
        meson = Meson(self)
        meson.configure()
        meson.build()
