#!/bin/bash

# clean up everything
rm -f ./**.gcno ./**.gcda ./**.c.gcov

# positive tests counters
declare -i total_pos=$(ls -1q ./func_tests/pos_*_args.txt | wc -l)
declare -i passed_pos=0

# test positive cases
printf "\n  Positive tests:\n\n"
for ((i = 1; i <= $total_pos; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" $i)
  expected=$(cat "./func_tests/pos_${n}_out.txt")
  args=$(cat "./func_tests/pos_${n}_args.txt")

  if command -v drmemory &> /dev/null
  then
    status=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- $args)
  elif command -v valgrind &> /dev/null
  then
    status=$(valgrind -q -- $args)
  else
    status=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- $args)
  fi
  actual=$(cat .temp)

  if [[ "$status" != "" ]]
  then
    print "    Test #$i: Failed. DrMemory output:\n\n$status\n"
  else
    diff -q "func_tests/pos_${n}_out.txt" ".temp"
    if [ $? -eq 0 ]
    then
      passed_pos=$(( passed_pos + 1 ))
      echo "    Test #$i: Passed."
    else
      echo "    Test #$i: Failed."
      echo "        Expected output: $expected"
      echo "        Actual output: $actual"
      exit 0
    fi
  fi
done

rm -f ./.temp

# negative tests counters
declare -i total_neg=$(ls -1q ./func_tests/neg_*_args.txt | wc -l)
declare -i passed_neg=0

# test negative cases
printf "\n  Negative tests:\n\n"
for ((i = 1; i <= $total_neg; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" $i)
  # expected=$(cat "./func_tests/neg_${n}_out.txt")
  args=$(cat "./func_tests/neg_${n}_args.txt")
  if command -v drmemory &> /dev/null
  then
    status=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- $args)
  elif command -v valgrind &> /dev/null
  then
    status=$(valgrind -q -- $args)
  else
    status=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- $args)
  fi

  if [[ "$status" != "" ]]
  then
    printf "    Test #$i: Failed. DrMemory output:\n\n$status\n" 
  else
    passed_neg=$(( passed_neg + 1 ))
    echo "    Test #$i: Passed."
  fi
done

rm -f ./.temp

echo
echo "Positive tests: $total_pos total, $passed_pos passed, $(( total_pos - passed_pos )) failed."
echo "Negative tests: $total_neg total, $passed_neg passed, $(( total_neg - passed_neg )) failed."

printf "\n  Gathering code coverage info...\n\n"

declare -i percentage=0
declare -i amount=$(ls -1q src/**.c | wc -l)
for f in src/**.c; do
  percentage_i=`gcov -s src -o obj $f | grep -E -o "[0-9]+" | head -1`
  percentage=$(( percentage + percentage_i / amount ))
done
echo "Code executed: ${percentage}%"

# move *.c.gcov files to appropriate folder
for f in $(ls ./*.gcov 2>/dev/null)
do
  mkdir -p ./func_tests/gcov
  mv $f ./func_tests/gcov/$(basename $f)
done
