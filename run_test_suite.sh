#!/bin/bash

cd ../..

make "test^cpp2^step0"
make "test^cpp2^step1"
make "test^cpp2^step2"
make "test^cpp2^step3"
make "test^cpp2^step4"
make "test^cpp2^step5"

cd ./impls/cpp2

make clean