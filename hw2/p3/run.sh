#!/bin/bash
#$ -V #Inherit the submission environment
#$ -cwd # Start job in submission directory
#$ -N cs392_search # Job Name
#$ -j y # Combine stderr and stdout
#$ -o $JOB_NAME.o$JOB_ID # Name of the output file (eg. myMPI.oJobID)
#$ -pe 2way 24 # Requests 2 MPI tasks/node, 24 cores total (that means the rest are evailable for OPENMP). this is a 2node submission.
#$ -q development # QUEUE name: alternatives: development, serial, gpu
#$ -l h_rt=00:10:00 # Run time (hh:mm:ss) - 10 minutes
#$ -M zhitingz@cs.utexas.edu # Address for email notification
#$ -m be # Email at Begin and End of job
set -x # Echo commands, use set echo with csh
ulimit -c unlimited

export KMP_AFFINITY=compact # This is to PIN OMP threads, only for intel compiler
export OMP_NUM_THREADS=6 # since we use 2MPI/node, this leaves 6 OMP threads per MPI task
ibrun tacc_affinity ./search 12 # Run the MPI executable named a.out with arguments 1000000


# use
# qsub lonestar.job
# to submit this script
# use showq to check the status
# use qdel to delete a job
