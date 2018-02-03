# OSQP-Wrapper
Simple C++ wrapper for [OSQP](http://osqp.readthedocs.io/en/latest/index.html) library.

## Dependeces
- [OSQP](http://osqp.readthedocs.io/en/latest/index.html) of course :smile:;
- [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page);
- [cmake](https://cmake.org/);
- [googletest](https://github.com/google/googletest) for testing.


## Build the library and the application
### Linux/macOs
```sh
git clone https://github.com/GiulioRomualdi/OSQP-Wrapper.git
cd OSQP-Wrapper
mkdir build && cd build
cmake -DOSQP_FOLDER=/path/where/osqp/was/cloned/ ../
make
[sudo] make install
```
If you want to enable tests set the `BUILD_TESTING` option to `ON`.

**Notice**: ``sudo`` is not necessary if you specify the ``CMAKE_INSTALL_PREFIX``. In this case it is necessary to add in the ``.bashrc`` the following lines:
### Linux
```
export OSQP-Wrapper_DIR=/path/where/you/installed/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OSQP-Wrapper_DIR/lib
```

### macOs
```
export OSQP-Wrapper_DIR=/path/where/you/installed/
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OSQP-Wrapper_DIR/lib
```
## API
[Here](https://giulioromualdi.github.io/OSQP-Wrapper/) you can find the documentation.
