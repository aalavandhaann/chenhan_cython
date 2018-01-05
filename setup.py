import os, platform, pkg_resources
from distutils.core import setup, Extension
from Cython.Distutils import build_ext

VERSION = (0, 0, 2)


data_dir = pkg_resources.resource_filename("autowrap", "data_files")
include_dir = os.path.join(data_dir, "autowrap")

ext = Extension("py_chenhancc",
                sources = ['src/py_chenhancc.cpp'],
                language="c++",
                extra_compile_args=["-std=c++14"],
                extra_link_args=["-std=c++14"],
                include_dirs = [include_dir, data_dir],
               )

setup(cmdclass={'build_ext':build_ext},
      name="py_chenhancc",
      version="%d.%d.%d" % VERSION,
      ext_modules = [ext],
      install_requires=[
          'autowrap',
      ],
      author='#0K Srinivasan Ramachandran',
      author_email='ashok.srinivasan2002@gmail.com',
      url='https://github.com/aalavandhaann/chenhan_cython',
      maintainer="#0K Srinivasan Ramachandran",
      maintainer_email="ashok.srinivasan2002@gmail.com",
      platforms=["any"],
      description='A geodesic path solution using chenhan',
      license='LICENSE.txt',
      keywords='geodesic mesh mesh3d opengl pygl triangle triangular meshes blender',
      python_requires='>=3',
      long_description=open('README.rst').read(),
      zip_safe=False,
     )

###AUTOWRAP
#autowrap --out py_chenhan.pyx chenhancc.pxd
#python setup.py build_ext --inplace