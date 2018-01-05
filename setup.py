import os, platform, pkg_resources
from distutils.core import setup, Extension
from Cython.Distutils import build_ext

data_dir = pkg_resources.resource_filename("autowrap", "data_files")
include_dir = os.path.join(data_dir, "autowrap")

CPP_suffix = "_GCC.cpp";
PYX_suffix = "_GCC.pyx";

if(platform.system() == 'WINDOWS'):
    CPP_suffix = "_WIN.cpp";
    PYX_suffix = "_WIN.pyx";

print('FOR THE DETECTED PLATFORM USE THE FILES ::: ');
print(CPP_suffix);
print(PYX_suffix);

ext = Extension("chenhancc",
                sources = ['chenhancc'+CPP_suffix],
                language="c++",
                extra_compile_args=["-std=c++14"],
                extra_link_args=["-std=c++14"],
                include_dirs = [include_dir, data_dir],
               )

setup(cmdclass={'build_ext':build_ext},
      name="chenhancc",
      version="0.0.1",
      ext_modules = [ext],
      install_requires=[
          'autowrap',
      ]
     )

