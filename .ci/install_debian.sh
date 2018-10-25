#!/bin/sh
set -e

apt-get update

# noninteractive tzdata ( https://stackoverflow.com/questions/44331836/apt-get-install-tzdata-noninteractive )
export DEBIAN_FRONTEND=noninteractive

# CI specific packages
apt-get install -y clang valgrind wget unzip

# Dependencies
# apt-get install -y build-essential libboost-system-dev libboost-filesystem-dev libboost-thread-dev cmake coinor-libipopt-dev liborocos-kdl-dev libeigen3-dev libtinyxml-dev libace-dev libgsl0-dev libopencv-dev libode-dev git swig qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev libqt5opengl5-dev libqcustomplot-dev libxml2-dev liburdfdom-dev

apt-get install -y build-essential cmake libeigen3-dev git
