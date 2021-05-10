#!/bin/sh

# Script created to make running POMCP easier.
# Juan Carlos Saborio, DFKI Labor Niedersachen
# 2021

# Change command line parameters accordingly

### Problem definition
problem=rocksample #Problem name: rocksample, cellar, etc.
size=7 # Grid size, eg.: 7x7, 11x11, etc.
number=8 #No. of rocks, bottles, etc.
number2=6 #No. of shelves (Cellar only)
number3=4 #No. of crates (Cellar only)
### Search and experiment parameters
mindoubles=2 #Begin at 2^x simulations, eg.: 2^0 = 1
maxdoubles=16 #Until 2^x simulations, eg.: 2^16 = 65536
numsteps=150 #How many steps is the agent allowed to take before the episode ends
runs=100 #How many episodes to run.  Useful to average performance.
rolloutknowledge=1 # 1 = random, 2 = preferred actions

outputfile=output.txt #Where to save performance summary.

./pomcp --problem $problem --size $size --number $number --number2 $number2 --number3 $number3 --mindoubles $mindoubles --maxdoubles $maxdoubles --numsteps $numsteps --runs $runs --rolloutknowledge $rolloutknowledge --outputfile $outputfile
