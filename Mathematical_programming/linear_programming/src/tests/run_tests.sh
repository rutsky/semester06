#!/bin/sh

SOLVER_BIN="../lp_problem"

if [ ! -e $SOLVER_BIN ]; then
  echo "Error: solver doesn't exist: " $SOLVER_BIN
fi

for f in *.in; do # TODO: not working if there is not `*.in' files.
  echo "   ###### Trying \`"$f"' #######   "
  $SOLVER_BIN $f
  if [ $? != 0 ]; then
    echo "Failed!"
    exit 1
  fi
done
