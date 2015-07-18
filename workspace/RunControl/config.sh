#!/bin/bash

CURRENTPATH="$(pwd -P)"

LCS_BASE=$CURRENTPATH/base
LCS_DEVICES=$CURRENTPATH/devices
LCS_SERVICES=$CURRENTPATH/services
LCS_LOGFILES=$CURRENTPATH/log/
LCS_DATA=$CURRENTPATH/data/

LCS_BINARYFILE='/tmp/'

export LCS_BASE
export LCS_DEVICES
export LCS_SERVICES
export LCS_LOGFILES
export LCS_DATA
export LCS_BINARYFILE
export PYTHONPATH=$PYTHONPATH:$CURRENTPATH #$LCS_BASE:$LCS_DEVICES:$LCS_SERVICES


