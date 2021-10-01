#!/bin/bash

## Provide a descriptive name
#PBS -N makejob

## Output and error files
#PBS -o makejob.out
#PBS -e makejob.err

## We need 1 node for this job
#PBS -l nodes=1

make all
