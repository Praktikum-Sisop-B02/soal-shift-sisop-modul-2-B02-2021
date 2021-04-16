#!/bin/bash

nf=$1
nc=$1".c"

gcc $nc -o $nf
./$nf
