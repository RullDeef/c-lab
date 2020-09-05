# clean up everything
rm -f ./**.gcno ./**.gcda ./**.c.gcov

# rebuild app
./build.sh

# positive tests counters
declare -i total_pos=$(ls -1q ./"func-tests"/pos_*_args.txt | wc -l)
declare -i passed_pos=0

# test positive cases
echo "  Positive tests:"
echo
for ((i = 1; i <= $total_pos; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" ${i})
  expected=$(cat "./func-tests/pos_${n}_out.txt")
  args=$(cat "./func-tests/pos_${n}_args.txt")
  ./bin/app.exe $args 2>.temp_${n} 1>.temp_${n}
  status=$?
  actual=$(cat .temp_${n})

  if [ $status -ne 0 ]
  then
    echo "Test #${i}: Failed with exit code $status" 
  else
    $(diff -q "./func-tests/pos_${n}_out.txt" ".temp_${n}")
    if [ $? -eq 0 ]
    then
      passed_pos=$(( passed_pos + 1 ))
      echo "Test #${i}: Passed."
    else
      echo "Test #${i}: Failed."
      echo "Expected output: $expected"
      echo "Actual output: $actual"
    fi
  fi
done

rm -rf ./.temp_*

# negative tests counters
declare -i total_neg=$(ls -1q ./"func-tests"/neg_*_args.txt | wc -l)
declare -i passed_neg=0

# test negative cases
echo
echo "  Negative tests:"
echo
for ((i = 1; i <= $total_neg; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" ${i})
  expected=$(cat "./func-tests/neg_${n}_out.txt")
  args=$(cat "./func-tests/neg_${n}_args.txt")
  ./bin/app.exe $args 2>.temp_${n} 1>.temp_${n}
  status=$?
  actual=$(cat .temp_${n})

  if [ $status = 0 ]
  then
    echo "Test #${i}: Failed with exit code $status" 
  else
    passed_neg=$(( passed_neg + 1 ))
    echo "Test #${i}: Passed."
  fi
done

rm -rf ./.temp_*

echo
echo "Positive tests: ${total_pos} total, ${passed_pos} passed, $(( total_pos - passed_pos )) failed."
echo "Negative tests: ${total_neg} total, ${passed_neg} passed, $(( total_neg - passed_neg )) failed."

echo
echo "Gathering code coverage info..."

cd ./src

declare -i percentage=0
declare -i amount=$(ls -1q ./**.c | wc -l)
for f in ./**.c; do
  percentage_i=`gcov $f | grep -E -o "[0-9]+" | head -1`
  percentage=$(( percentage + percentage_i / amount ))
done
echo "Code executed: ${percentage}%"

# clean up everything except *.c.gcov
rm -f *.gcno *.gcda

# move *.c.gcov files to appropriate folder
for f in $(ls ./**gcov)
do
  mkdir --parents ../func-tests/gcov/$(dirname $f)
  mv $f ../func-tests/gcov/$(basename $f)
done
