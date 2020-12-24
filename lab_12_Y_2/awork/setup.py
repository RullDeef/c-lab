from distutils.core import setup, Extension

setup(
    name="awork",
    version="1.0.0",
    description="tiny module implemented in C",
    author="Klimenko Alexey",
    author_email="klimenko0037@gmail.com",
    ext_modules=[Extension("awork", ["./src/awork.c"])]
)
