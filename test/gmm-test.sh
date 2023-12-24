#!/bin/sh

ulimit -c 10000

export GMM_FAULT_EXEC=./gmm-exec-fault.sh

LD_PRELOAD=../gmm/.libs/libgmm.so ./gmm-test $*

