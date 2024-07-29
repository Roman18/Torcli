#!/bin/bash


export LD_PRELOAD= #full path to the torcli.so

${@}

unset LD_PRELOAD
