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
import sys, os, subprocess, warnings, re


class build_ext_subclass( build_ext ):
    def build_extensions(self):
        is_msvc = self.compiler.compiler_type.lower() == 'msvc'
        is_clang = hasattr(self.compiler, 'compiler_cxx') and ("clang++" in self.compiler.compiler_cxx)

        if not is_msvc:
            self.add_march_native()
        self.add_restrict_qualifier()

        if is_msvc:
            for e in self.extensions:
                e.extra_compile_args += ['/O2', '/std:c++14']
                e.define_macros += [("_CRT_SECURE_NO_WARNINGS", None)]
        elif is_clang:
            for e in self.extensions:
                e.extra_compile_args += ['-O2', '-std=c++17']
        else:
            ### here only 'mingw32' should be a valid option, the rest are set there
            ### just in case
            if (
                  (is_windows) and
                  (self.compiler.compiler_type.lower()
                   in ["mingw32", "mingw64", "mingw", "msys", "msys2", "gcc", "g++"])
                ):
                for e in self.extensions:
                    e.extra_compile_args += ['-O2', '-std=gnu++11']
                    e.define_macros += [("_FILE_OFFSET_BITS", 64),
                                        ("__STDC_FORMAT_MACROS", None)]
                    if use_ansi_stdio:
                        e.define_macros += [("__USE_MINGW_ANSI_STDIO", None)]
                    else:
                        e.define_macros += [("AVOID_MINGW_ANSI_STDIO", None)]
            else:
                for e in self.extensions:
                    e.extra_compile_args += ['-O2', '-std=c++11']
                    # e.extra_compile_args = ["-std=c++11", "-fsanitize=address", "-static-libasan", "-ggdb"]
                    # e.extra_link_args    = ["-fsanitize=address", "-static-libasan"]
        build_ext.build_extensions(self)

    def add_march_native(self):
        arg_march_native = "-march=native"
        arg_mcpu_native = "-mcpu=native"
        if self.test_supports_compile_arg(arg_march_native):
            for e in self.extensions:
                e.extra_compile_args.append(arg_march_native)
        elif self.test_supports_compile_arg(arg_mcpu_native):
            for e in self.extensions:
                e.extra_compile_args.append(arg_mcpu_native)

    def test_supports_compile_arg(self, comm):
        is_supported = False
        try:
            if not hasattr(self.compiler, "compiler_cxx"):
                return False
            if not isinstance(comm, list):
                comm = [comm]
            print("--- Checking compiler support for option '%s'" % " ".join(comm))
            fname = "readsparse_compiler_testing.cpp"
            with open(fname, "w") as ftest:
                ftest.write(u"int main(int argc, char**argv) {return 0;}\n")
            try:
                cmd = [self.compiler.compiler_cxx[0]]
            except:
                cmd = list(self.compiler.compiler_cxx)
            val_good = subprocess.call(cmd + [fname])
            try:
                val = subprocess.call(cmd + comm + [fname])
                is_supported = (val == val_good)
            except:
                is_supported = False
        except:
            pass
        try:
            os.remove(fname)
        except:
            pass
        return is_supported

    def add_restrict_qualifier(self):
        supports_restrict = False
        try:
            if not hasattr(self.compiler, "compiler_cxx"):
                return None
            print("--- Checking compiler support for '__restrict' qualifier")
            fname = "readsparse_compiler_testing.cpp"
            with open(fname, "w") as ftest:
                ftest.write(u"int main(int argc, char**argv) {return 0;}\n")
            try:
                cmd = [self.compiler.compiler_cxx[0]]
            except:
                cmd = list(self.compiler.compiler_cxx)
            val_good = subprocess.call(cmd + [fname])
            try:
                with open(fname, "w") as ftest:
                    ftest.write(u"int main(int argc, char**argv) {double *__restrict x = nullptr; return 0;}\n")
                val = subprocess.call(cmd + [fname])
                supports_restrict = (val == val_good)
            except:
                return None
        except:
            pass
        try:
            os.remove(fname)
        except:
            pass
        
        if supports_restrict:
            for e in self.extensions:
                e.define_macros += [("SUPPORTS_RESTRICT", "1")]


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
    version = '0.1.5',
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
