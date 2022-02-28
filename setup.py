import numpy as np
from distutils.core import setup, Extension
# patch to compile with mingw
import distutils.cygwinccompiler
distutils.cygwinccompiler.get_msvcr = lambda: []

version_string = '0.0.1'
ver_major, ver_minor, ver_bugfix = version_string.split('.')

extension = Extension(
    name = 'dummy',
    sources = ['./src/dummy_module.c'],
    include_dirs = ['./include', np.get_include()],
    define_macros = [('MAJOR_VERSION' , ver_major),
                     ('MINOR_VERSION' , ver_minor),
                     ('BUGFIX_VERSION', ver_bugfix)],
    language='C'

)

setup(
    name = 'dummy',
    version = version_string,
    author = 'hosein',
    url = 'https://github.com/hos-b',
    long_description = "demo C extension",
    ext_modules = [extension],
    packages = ['dummy'],
    package_dir = {'dummy' : './stub'},
    package_data = {
        'dummy': ['__init__.pyi', 'py.typed']
    }
)