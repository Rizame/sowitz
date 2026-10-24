#!/bin/bash
set -e

rm -rf build/
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
