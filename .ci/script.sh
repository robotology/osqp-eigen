#!/bin/sh
set -e

# google test
wget https://github.com/google/googletest/archive/release-1.8.0.zip
unzip release-1.8.0.zip
cd googletest-release-1.8.0
mkdir build
cd build
cmake -G"${TRAVIS_CMAKE_GENERATOR}" -DBUILD_GTEST=ON -DBUILD_SHARED_LIBS=ON ..
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
cd ../..

# osqp
git clone --recursive https://github.com/oxfordcontrol/osqp.git
cd osqp
mkdir build
cd build
cmake -G"${TRAVIS_CMAKE_GENERATOR}" -DCMAKE_BUILD_TYPE=${TRAVIS_BUILD_TYPE} -DUNITTESTS=OFF ..
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
cd ../..

# Build, test and install osqp-eigen
cd $TRAVIS_BUILD_DIR
mkdir build && cd build
cmake -G"${TRAVIS_CMAKE_GENERATOR}" -DCMAKE_BUILD_TYPE=${TRAVIS_BUILD_TYPE} -DBUILD_TESTING:BOOL=ON ..
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
make test
