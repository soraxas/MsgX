import re
import os
import glob
import shutil
from setuptools import setup, find_packages


this_directory = os.path.abspath(os.path.dirname(__file__))


# read the contents of README file
with open(os.path.join(this_directory, "README.md"), encoding="utf-8") as f:
    long_description = f.read()

# read the version file
VERSIONFILE = "python/_version.py"
verstrline = open(VERSIONFILE, "rt").read()
mo = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]", verstrline, re.M)
if not mo:
    raise RuntimeError("Unable to find version string in %s." % (VERSIONFILE,))
version_str = mo.group(1)


# copy capnp file from c++ source to python package location
src_glob = glob.glob(
    os.path.join(this_directory, "c++", "src", "msgx_capnp", "*.capnp")
)
dst_dir = os.path.join(this_directory, "python")
for file in src_glob:
    print("copying {} -> {}".format(file, dst_dir))
    shutil.copy(file, dst_dir)

setup(
    name="msgx",
    version=version_str,
    description="Message Exchange protocol.",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="Tin Lai (@soraxas)",
    author_email="oscar@tinyiu.com",
    license="MIT",
    url="https://github.com/soraxas/msgx",
    keywords="msgx deserialise c++ capnp protobuf zmq",
    python_requires=">=3.5",
    # do include package data in distn
    include_package_data=True,
    # telling setup.py that things within python/ is for package msgx
    package_dir={"msgx": "python"},
    # telling setup.py that msgx is a package that we want
    packages=["msgx"],
    #
    install_requires=[
        "pyzmq>=25.0",
        "pycapnp>=1.1.0",
        "numpy",
    ],
    classifiers=[
        "Intended Audience :: Developers",
        "Intended Audience :: End Users/Desktop",
        "Intended Audience :: System Administrators",
        "License :: OSI Approved :: MIT License",
        "Operating System :: MacOS",
        "Operating System :: POSIX",
        "Operating System :: Unix",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Topic :: Desktop Environment",
        "Topic :: Terminals",
        "Topic :: Utilities",
    ],
)
