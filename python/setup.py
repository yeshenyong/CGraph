# sudo python3 setup.py bdist_wheel
# sudo pip3 install -I dist/pyCGraph-0.0.1-cp36-cp36m-linux_x86_64.whl
import glob
import os
from setuptools import setup, Extension, find_packages

root_dir = os.path.abspath(os.path.join(os.getcwd()))

include_dirs = []
extra_compile_args = []

extra_compile_args.append("-pthread")
extra_compile_args.append("-std=c++11")
extra_compile_args.append('-fvisibility=hidden')

include_dirs.append("/usr/include")
include_dirs.append("/usr/local/include")
include_dirs.append("../third_party/pybind11/include")
include_dirs.append("../src")

sources = [root_dir + '/PyWrapper/py_export.cpp'] + glob.glob("../src/**/*.cpp", recursive=True)

ext_modules = [
    Extension(
        "pyCGraph",
        sources,
        extra_compile_args=extra_compile_args,
        include_dirs=include_dirs
    ),
]

setup(name="pyCGraph",        # package name
      version="0.0.1",      # version
      description="Python Interface for CGraph",
      url="https://github.com/ChunelFeng/CGraph",
      ext_package='CGraph',
      packages=find_packages(where='.'),
      package_data={'': ['python/lib/lib*.so*']},
      ext_modules=ext_modules)
