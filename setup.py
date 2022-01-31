from setuptools import Extension, setup
from distutils.ccompiler import get_default_compiler

compiler = get_default_compiler()

pypdf_tin_ext = Extension(
    name = 'pypdf_tin',
    sources = [
        'src/cpp/PdfDocument.cpp', 'src/cpp/PyBytesPtr.cpp',
        'src/cpp/PyStringPtr.cpp', 'src/cpp/pypdf_tin.cpp',
        'src/cpp/Page.cpp', 'src/cpp/ImmutableLazyList.cpp',
        'src/cpp/Color.cpp', 'src/cpp/Text.cpp', 'src/cpp/TextStyle.cpp',
        'src/cpp/TextStyleSegment.cpp'
    ],
    include_dirs = [ 'src/cpp', 'pdf_tin/src/main/cpp',
                     '/usr/include/glib-2.0',
                     '/usr/lib/x86_64-linux-gnu/glib-2.0/include' ],
    library_dirs = [ 'pdf_tin/build/src/main/cpp' ],
    libraries = [ 'pdf_tin', 'poppler-glib', 'poppler', 'gio-2.0',
                  'gobject-2.0', 'glib-2.0', 'cairo', 'freetype',
                  'fontconfig', 'jpeg', 'openjp2', 'png16', 'z' ],
    extra_compile_args = [ '-std=c++17', '-g' ]
)

setup(
    name = 'pypdf_tin',
    version = '0.0.1',
    description = 'Python interface to Poppler',
    long_description = '''
Python interface to Poppler
''',
    author = 'Tom Ault',
    author_email = 'tomault@cs.cmu.edu',
    ext_modules = [ pypdf_tin_ext ]
)
