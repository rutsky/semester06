#!/bin/sh

set -e

SOLVER_DIR="../"
SOLVER_BIN="lp_problem"
TESTS_DIR=`pwd`

if [ ! -e $SOLVER_BIN ]; then
  echo "Error: solver doesn't exist: " $SOLVER_BIN
fi

for f in *.in; do # TODO: not working if there is not `*.in' files.
  echo "   ###### Trying \`"$f"' #######   "
  pushd .
  cd $SOLVER_DIR
  ./$SOLVER_BIN $TESTS_DIR/$f
  popd
  if [ $? != 0 ]; then
    echo "Failed!"
    exit 1
  fi
done

echo
echo
echo "All tests successfully passed!"
