#!/usr/bin/env bash

echo "### STARTING TESTS ###"

# compile
gcc ./test_chacha.c ./chacha20.c -ov ./bin/test_chacha

# execute
./bin/test_chacha

echo "... TESTS RAN ..."
