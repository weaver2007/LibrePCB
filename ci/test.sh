#!/usr/bin/env bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -eufv -o pipefail

# run tests
if [ "${TRAVIS_OS_NAME-}" = "linux" ]
then
  xvfb-run --server-args="-screen 0 1024x768x24" ./build/generated/unix/qztest
  xvfb-run --server-args="-screen 0 1024x768x24" ./build/generated/unix/tests
elif [ "${TRAVIS_OS_NAME-}" = "osx" ]
then
  ./build/generated/mac/qztest
  ./build/generated/mac/tests
else
  ./build/generated/windows/qztest.exe
  ./build/generated/windows/tests.exe
fi

