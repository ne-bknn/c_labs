from distutils.core import setup, Extension
setup(name="pystrassen", version="1.0", ext_modules=[Extension("pystrassen", ["pystrassen.c"])])
