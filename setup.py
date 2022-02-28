import numpy as np
from distutils.core import setup, Extension
# patch to compile with mingw
import distutils.cygwinccompiler
distutils.cygwinccompiler.get_msvcr = lambda: []

version_string = '0.0.1'
ver_maj, ver_min, ver_fix = version_string.split('.')

extension = Extension(
    name = 'dummy',
    sources = ['./src/dummy_module.c'],
    include_dirs = ['./include', np.get_include()],
    define_macros = [('MAJOR_VERSION' , ver_maj),
                     ('MINOR_VERSION' , ver_min),
                     ('BUGFIX_VERSION', ver_fix)],
)

setup(
    name='dummy',
    version=version_string,
    ext_modules=[extension]
)