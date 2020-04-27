import os, platform, pkg_resources
#from distutils.core import setup, Extension
from setuptools import setup, Extension;
from Cython.Distutils import build_ext

NO_AUTOWRAP=False;
try:
    import autowrap;
except ImportError:
    NO_AUTOWRAP=True;

VERSION = (0, 0, 6);


data_dir = pkg_resources.resource_filename("autowrap", "data_files")
include_dir = os.path.join(data_dir, "autowrap")

if(NO_AUTOWRAP):
    raise IndentationError;
else:
    print('FOUND AUTOWRAP LIBRARY, PROCEED');

print('PRECOMPILE USING AUTOWRAP BEFORE PROCEEDING');
from autowrap.Main import run as autowrap_run;
autowrap_run([os.path.abspath('./src/chenhancc.pxd')], [], [], os.path.abspath('./src/py_chenhancc.pyx'), );


# if(not os.path.exists(os.path.abspath('./py_chenhancc.cpp'))):
#     import subprocess;
#     source_files_dir = os.path.abspath("./src");
#     source_file_pxd = os.path.join(source_files_dir, "chenhancc.pxd");
#     out_file_pyx = os.path.join(source_files_dir, "py_chenhancc.pyx");
#     
#     subprocess.Popen(" ".join(["autowrap", "--out", out_file_pyx, source_file_pxd]), cwd=os.path.abspath("./src"));
# #     autowrap --out py_chenhancc.pyx chenhancc.pxd

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
      install_requires=['autowrap>=0.22'],
      author='#0K Srinivasan Ramachandran',
      author_email='ashok.srinivasan2002@gmail.com',
      url='https://github.com/aalavandhaann/chenhan_cython',
      maintainer="#0K Srinivasan Ramachandran",
      maintainer_email="ashok.srinivasan2002@gmail.com",
      platforms=["any"],
      description='A geodesic path solution using chenhan',
      license='LICENSE.txt',
      keywords='geodesic mesh mesh3d opengl pygl triangle triangular meshes blender',
      python_requires='>=2',
      long_description=open('README.md').read(),
      long_description_content_type='text/markdown',
      zip_safe=False,
     )

###AUTOWRAP
#autowrap --out py_chenhan.pyx chenhancc.pxd
#python setup.py build_ext --inplace