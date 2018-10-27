#!/bin/bash
set -euo pipefail
DEST=$1
cmake ./ -DCMAKE_INSTALL_PREFIX=$1
make install
