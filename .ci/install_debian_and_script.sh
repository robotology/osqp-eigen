#!/bin/sh
set -e

DIR=$(dirname "$(readlink -f "$0")")

sh $DIR/install_debian.sh
sh $DIR/script.sh
