import setuptools

# with open("README.md", "r", encoding="utf-8") as fh:
#    long_description = fh.read()
long_description = ""

setuptools.setup(
    name="plotmsg_dash",
    version="0.7.1",
    author="Tin Lai",
    author_email="oscar@tinyiu.com",
    description="Rendering backend for PlotMsg from C++",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/pypa/sampleproject",
    project_urls={
        "Bug Tracker": "https://github.com/pypa/sampleproject/issues",
    },
    install_requires=[
        "protobuf>=4.0",
        "plotly",
        "zmq",
        "ipython",
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    package_dir={"": "plotmsg_dash"},
    packages=setuptools.find_packages(where="plotmsg_dash"),
    python_requires=">=3.6",
)
