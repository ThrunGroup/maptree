from glob import glob
from setuptools import setup
try:
    from pybind11.setup_helpers import Pybind11Extension
except ImportError:
    from setuptools import Extension as Pybind11Extension
from os.path import join

__version__ = "0.0.1"


class get_pybind_include(object):
    """
    Helper class to determine the pybind11 include path.
    The purpose of this class is to postpone importing pybind11
    until it is actually installed via dataloader's setup_requires arg,
    so that the ``get_include()`` method can be invoked.
    """
    def __str__(self):
        import pybind11

        return pybind11.get_include()


ALL_SOURCE_FILES = sorted(
    glob(join("maptree", "src", "*", "*.cpp"))
    + glob(join("maptree", "src", "*.cpp"))
)

ALL_HEADER_FILES = sorted(
    glob(join("maptree", "include", "*", "*.h"))
    + glob(join("maptree", "include", "*.h"))
)

ext_modules = [
    Pybind11Extension(
        "maptree",
        ALL_SOURCE_FILES,
        include_dirs=[get_pybind_include(), join("maptree", "include")],
        extra_compile_args=['-O3', '-DNDEBUG'],
    ),
]

setup(
    name="maptree",
    version=__version__,
    author="Redacted",
    maintainer="Redacted",
    author_email="Redacted",
    ext_modules=ext_modules,
    setup_requires=[
        "pybind11>=2.5.0",
        "numpy>=1.18",
    ],
    include_package_data=True,
    zip_safe=False,
    headers=ALL_HEADER_FILES,
)