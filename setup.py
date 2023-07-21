try:
    from setuptools import setup
    from setuptools.extension import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension

import numpy as np
import sys, ctypes
from Cython.Distutils import build_ext
from platform import architecture
from os import environ
import sys, os, subprocess, warnings, re

## Modify this to make the output of the compilation tests more verbose
silent_tests = not (("verbose" in sys.argv)
                    or ("-verbose" in sys.argv)
                    or ("--verbose" in sys.argv))

## Workaround for python<=3.9 on windows
try:
    EXIT_SUCCESS = os.EX_OK
except AttributeError:
    EXIT_SUCCESS = 0

class build_ext_subclass( build_ext ):
    def build_extensions(self):
        is_msvc = self.compiler.compiler_type.lower() == 'msvc'
        is_clang = hasattr(self.compiler, 'compiler_cxx') and ("clang++" in self.compiler.compiler_cxx)

        if (not is_msvc) and (not self.check_for_variable_dont_set_march() and not self.check_cflags_contain_arch()):
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
                self.add_O2()
                self.add_std_cpp11()
                # for e in self.extensions:
                    # e.extra_compile_args = ["-std=c++11", "-fsanitize=address", "-static-libasan", "-ggdb"]
                    # e.extra_link_args    = ["-fsanitize=address", "-static-libasan"]
        build_ext.build_extensions(self)

    def check_cflags_contain_arch(self):
        if ("CFLAGS" in os.environ) or ("CXXFLAGS" in os.environ):
            has_cflags = "CFLAGS" in os.environ
            has_cxxflags = "CXXFLAGS" in os.environ
            arch_list = [
                "-march", "-mcpu", "-mtune", "-msse", "-msse2", "-msse3",
                "-mssse3", "-msse4", "-msse4a", "-msse4.1", "-msse4.2",
                "-mavx", "-mavx2", "-mavx512"
            ]
            for flag in arch_list:
                if has_cflags and flag in os.environ["CFLAGS"]:
                    return True
                if has_cxxflags and flag in os.environ["CXXFLAGS"]:
                    return True
        return False

    def check_for_variable_dont_set_march(self):
        return "DONT_SET_MARCH" in os.environ

    def add_march_native(self):
        is_apple = sys.platform[:3].lower() == "dar"
        args_march_native = ["-march=native", "-mcpu=native"]
        for arg_march_native in args_march_native:
            if self.test_supports_compile_arg(arg_march_native, with_c_comp=is_apple):
                for e in self.extensions:
                    e.extra_compile_args.append(arg_march_native)
                break

    def add_O2(self):
        arg_O2 = "-O2"
        if self.test_supports_compile_arg(arg_O2):
            for e in self.extensions:
                e.extra_compile_args.append(arg_O2)
                e.extra_link_args.append(arg_O2)

    def add_std_cpp11(self):
        arg_std_cpp11 = "-std=c++11"
        if self.test_supports_compile_arg(arg_std_cpp11):
            for e in self.extensions:
                e.extra_compile_args.append(arg_std_cpp11)
                e.extra_link_args.append(arg_std_cpp11)

    # Note: in apple systems, it somehow might end up triggering the arguments with
    # the C compiler instead of the CXX compiler. What's worse, sometimes this compiler
    # thinks it's building for aarch64 even when executed in amd64.
    def test_supports_compile_arg(self, comm, with_omp=False, with_c_comp=False):
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
                if not isinstance(self.compiler.compiler_cxx, list):
                    cmd = list(self.compiler.compiler_cxx)
                else:
                    cmd = self.compiler.compiler_cxx
            except Exception:
                cmd = self.compiler.compiler_cxx
            if with_c_comp:
                if not isinstance(self.compiler.compiler, list):
                    cmd0 = list(self.compiler.compiler)
                else:
                    cmd0 = self.compiler.compiler
            if with_omp:
                with open(fname, "w") as ftest:
                    ftest.write(u"#include <omp.h>\nint main(int argc, char**argv) {return 0;}\n")
            try:
                val = subprocess.run(cmd + comm + [fname], capture_output=silent_tests).returncode
                is_supported = (val == EXIT_SUCCESS)
                if is_supported and with_c_comp:
                    val = subprocess.run(cmd0 + comm + [fname], capture_output=silent_tests).returncode
                    is_supported = (val == EXIT_SUCCESS)
            except Exception:
                is_supported = False
        except Exception:
            pass
        try:
            os.remove(fname)
        except Exception:
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
                if not isinstance(self.compiler.compiler_cxx, list):
                    cmd = list(self.compiler.compiler_cxx)
                else:
                    cmd = self.compiler.compiler_cxx
            except Exception:
                cmd = self.compiler.compiler_cxx
            try:
                with open(fname, "w") as ftest:
                    ftest.write(u"int main(int argc, char**argv) {double *__restrict x = 0; return 0;}\n")
                val = subprocess.run(cmd + comm + [fname], capture_output=silent_tests).returncode
                is_supported = (val == EXIT_SUCCESS)
            except Exception:
                return None
        except Exception:
            pass
        try:
            os.remove(fname)
        except Exception:
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
    version = '0.1.5-10',
    description = 'Read and Write Sparse Matrices in Text Format',
    author = 'David Cortes',
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
                                define_macros=[
                                    ("_FOR_PYTHON", None),
                                    ("NDEBUG", None),
                                    ("NPY_NO_DEPRECATED_API", "NPY_1_7_API_VERSION")
                                ],
                                compiler_directives={'language_level' : "3"}
                            )]
    )
