# osqp-eigen

|  General  | [![c++14](https://img.shields.io/badge/standard-C++14-blue.svg?style=flat&logo=c%2B%2B)](https://isocpp.org) [![License](https://img.shields.io/badge/License-BSD_3--Clause-orange.svg)](https://github.com/robotology/osqp-eigen/blob/master/LICENSE) |
| :-------: | :----------------------------------------------------------: |
| **CI/CD** | [![Codacy Badge](https://app.codacy.com/project/badge/Grade/a18710c10f1c4df19bc2759fd50e9cf5)](https://www.codacy.com/gh/robotology/osqp-eigen/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=robotology/osqp-eigen&amp;utm_campaign=Badge_Grade) [![CI](https://github.com/robotology/osqp-eigen/workflows/C++%20CI%20Workflow/badge.svg)](https://github.com/robotology/osqp-eigen/workflows/C++%20CI%20Workflow/badge.svg) [![Azure](https://dev.azure.com/conda-forge/feedstock-builds/_apis/build/status/osqp-eigen-feedstock?branchName=master)](https://dev.azure.com/conda-forge/feedstock-builds/_build/results?buildId=341091&view=results) |
| **conda** | [![Conda Recipe](https://img.shields.io/badge/recipe-osqp--eigen-green.svg)](https://anaconda.org/conda-forge/osqp-eigen)  [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/osqp-eigen.svg)](https://anaconda.org/conda-forge/osqp-eigen)  [![Conda Version](https://img.shields.io/conda/vn/conda-forge/osqp-eigen.svg)](https://anaconda.org/conda-forge/osqp-eigen)  [![Conda Platforms](https://img.shields.io/conda/pn/conda-forge/osqp-eigen.svg)](https://anaconda.org/conda-forge/osqp-eigen) |



Simple C++ wrapper for [osqp](http://osqp.readthedocs.io/en/latest/index.html) library.

## 📚 Documentation
The documentation is available online at the accompanying [website](https://robotology.github.io/osqp-eigen).


## 📄 Dependences
The project depends only on [`osqp`](http://osqp.readthedocs.io/en/latest/index.html) and [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page). Please install [Catch2](https://github.com/catchorg/Catch2)  if you want to run the tests only for testing.

## 🛠️ Usage

### 📦 Install with conda (recommended)
You can easily the library with [`conda`](https://github.com/conda-forge/osqp-eigen-feedstock) using the following command
```
conda install -c conda-forge osqp-eigen
```
`conda` will automatically install [`osqp`](http://osqp.readthedocs.io/en/latest/index.html) and [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page).

### ⚙️ Build from source (advanced)

1. Clone the repository
   ```
   git clone https://github.com/robotology/osqp-eigen.git
   ```
2. Build it
   ```
   cd osqp-eigen
   mkdir build
   cd build
   cmake -DCMAKE_INSTALL_PREFIX:PATH=<custom-folder> ../
   make
   make install
   ```
3. Add the following environmental variable
   ```
   OsqpEigen_DIR=/path/where/you/installed/
   ```

## 🖥️ How to use the library
**osqp-eigen** provides native `CMake` support which allows the library to be easily used in `CMake` projects.
**osqp-eigen** exports a CMake target called `OsqpEigen::OsqpEigen` which can be imported using the `find_package` CMake command and used by calling `target_link_libraries` as in the following example:
```cmake
cmake_minimum_required(VERSION 3.0)
project(myproject)
find_package(OsqpEigen REQUIRED)
add_executable(example example.cpp)
target_link_libraries(example OsqpEigen::OsqpEigen)
```

##  🐛 Bug reports and support
All types of [issues](https://github.com/robotology/osqp-eigen/issues/new) are welcome.

## 📝 License
Materials in this repository are distributed under the following license:

> All software is licensed under the BSD 3-Clause License. See [LICENSE](https://github.com/robotology/osqp-eigen/blob/master/LICENSE) file for details.
