#!/bin/bash

## Provide a descriptive name for the job
#PBS -N testjob

## Output and error files
#PBS -o testjob.out
#PBS -e testjob.err

## Limit runtime
#PBS -l walltime=01:00:00

## How many nodes:processors_per_node do we need?
#PBS -l nodes=2:ppn=4

## Start
##echo "PBS_NODEFILE = $PBS_NODEFILE"
##cat $PBS_NODEFILE

## Run the job (use full paths to make sure we execute the correct executable
## Path: FIXME to lead to your executable

###TODO
