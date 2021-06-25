try:
    from setuptools import setup
    from setuptools.extension import Extension
except:
    from distutils.core import setup
    from distutils.extension import Extension

import numpy as np
import sys, ctypes
from Cython.Distutils import build_ext
from platform import architecture
from os import environ

class build_ext_subclass( build_ext ):
    def build_extensions(self):
        c = self.compiler.compiler_type.lower()
        if c == 'msvc': # visual studio
            for e in self.extensions:
                e.extra_compile_args = ['/O2', '/std:c++14']
                e.define_macros += [("_CRT_SECURE_NO_WARNINGS", None)]
        elif (c == "clang") or (c == "clang++"):
            for e in self.extensions:
                e.extra_compile_args = ['-O2', '-march=native', '-std=c++17']
        else:
            ### here only 'mingw32' should be a valid option, the rest are set there
            ### just in case
            if (
                  (is_windows) and
                  (c in ["mingw32", "mingw64", "mingw", "msys", "msys2", "gcc", "g++"])
                ):
                for e in self.extensions:
                    e.extra_compile_args = ['-O2', '-march=native', '-std=gnu++11']
                    e.define_macros += [("_FILE_OFFSET_BITS", 64),
                                        ("__STDC_FORMAT_MACROS", None)]
                    if use_ansi_stdio:
                        e.define_macros += [("__USE_MINGW_ANSI_STDIO", None)]
                    else:
                        e.define_macros += [("AVOID_MINGW_ANSI_STDIO", None)]
            else:
                for e in self.extensions:
                    e.extra_compile_args = ['-O2', '-march=native', '-std=c++11']
                    # e.extra_compile_args = ["-std=c++11", "-fsanitize=address", "-static-libasan", "-ggdb"]
                    # e.extra_link_args    = ["-fsanitize=address", "-static-libasan"]
        build_ext.build_extensions(self)


args_ansi_stdio = ["ansistdio", "-ansistdio", "--ansistdio"]
use_ansi_stdio = False
for a in args_ansi_stdio:
    if a in sys.argv:
        use_ansi_stdio = True
        break
if use_ansi_stdio:
    sys.argv = [a for a in sys.argv if a not in args_ansi_stdio]
if environ.get('ANSISTDIO') is not None:
    use_ansi_stdio = True

platform_is_32 = False
if architecture()[0] == '32bit':
    platform_is_32 = True
elif np.iinfo(ctypes.c_size_t).max < np.iinfo(ctypes.c_uint64).max:
    platform_is_32 = True

is_windows = sys.platform[:3] == "win"

setup(
    name  = "readsparse",
    packages = ["readsparse"],
    version = '0.1.4',
    description = 'Read and Write Sparse Matrices in Text Format',
    author = 'David Cortes',
    author_email = 'david.cortes.rivera@gmail.com',
    url = 'https://github.com/david-cortes/readsparse',
    keywords = ['sparse', 'svmlight', 'libsvm'],
    cmdclass = {'build_ext': build_ext_subclass},
    ext_modules = [Extension(
                                "readsparse._cpp_interface",
                                sources=["readsparse/cpp_interface_uint64_t.pyx"
                                         if platform_is_32 else 
                                         "readsparse/cpp_interface_size_t.pyx",
                                         "src/reader.cpp", "src/writer.cpp"
                                         ],
                                include_dirs=[np.get_include(), "./src", "./readsparse"],
                                language="c++",
                                install_requires=["numpy", "cython", "scipy"],
                                define_macros=[("_FOR_PYTHON", None)],
                                compiler_directives={'language_level' : "3"}
                            )]
    )
