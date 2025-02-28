from setuptools import setup
from torch.utils.cpp_extension import CppExtension, BuildExtension
import os

eigen_include_dir = '/usr/include/eigen3'  # Reemplaza con la ruta correcta

setup(
    name='sammon_module',
    ext_modules=[
        CppExtension(
            name='sammon_module',
            sources=['sammon.cpp'],
            include_dirs=[eigen_include_dir],
        ),
    ],
    cmdclass={
        'build_ext': BuildExtension
    }
)
