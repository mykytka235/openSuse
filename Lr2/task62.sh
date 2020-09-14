#!/bin/bash

find . -maxdepth 1 -type f -printf '%T+ %p\n' | sort | head -n 1
#array=($(ls -prt | grep -v / ))
#echo ${array[0]}  
