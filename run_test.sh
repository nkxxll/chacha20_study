#!/usr/bin/env bash

echo "### STARTING TESTS ###"

gcc ./test_chacha.c ./chacha20.c -o ./bin/test_chacha

echo "... TESTS RAN ..."
