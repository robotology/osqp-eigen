# osqp-eigen
Simple C++ wrapper for [osqp](http://osqp.readthedocs.io/en/latest/index.html) library.

| System        | Status                                                                                                                                                                      |
| ------------- | :-------------:                                                                                                                                                             |
| Linux / OSX   | [![Build Status](https://travis-ci.org/robotology/osqp-eigen.svg?branch=master)](https://travis-ci.org/robotology/osqp-eigen)                                               |
| Windows       | [![Build status](https://ci.appveyor.com/api/projects/status/1uecfmyvxb2dujt9/branch/master?svg=true)](https://ci.appveyor.com/project/robotology/osqp-eigen/branch/master) |


## Dependeces
- [osqp](http://osqp.readthedocs.io/en/latest/index.html) of course :smile:;
- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page);
- [cmake](https://cmake.org/);
- [googletest](https://github.com/google/googletest) (only for testing).

## Build the library and the application
### Linux / macOs
```sh
git clone https://github.com/robotology/osqp-eigen.git
cd osqp-eigen
mkdir build && cd build
cmake ../
make
[sudo] make install
```
If you want to enable tests set the `BUILD_TESTING` option to `ON`.

**Notice**: ``sudo`` is not necessary if you specify the ``CMAKE_INSTALL_PREFIX``. In this case it is necessary to add in the ``.bashrc`` the following lines:

### Linux / macOs
```
export OsqpEigen_DIR=/path/where/you/installed/
```

**Notice**:  The choice of `OsqpEigen_DIR` name for the environment variable is not random. Indeed `<package>_DIR` is one of the search paths of [`find_package()`](https://cmake.org/cmake/help/v3.0/command/find_package.html).

## How to use the wrapper
[Here](./example/) you can find a simple example that shows how to use this library.

## API
[Here](https://robotology.github.io/osqp-eigen/) you can find the documentation.

## Bug reports and support
All types of [issues](https://github.com/robotology/osqp-eigen/issues/new) are welcome :smile:.
