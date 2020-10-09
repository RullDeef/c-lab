#!/bin/bash

rm -rf ./func_tests/out
mkdir -p ./func_tests/out

# positive tests counters
declare -i total_pos=$(ls -1q ./func_tests/pos_*_args.txt 2>/dev/null | wc -l)
declare -i passed_pos=0

# test positive cases
printf "\n  Позитивные тесты:\n\n"
for ((i = 1; i <= $total_pos; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" $i)
  expected=$(cat "./func_tests/pos_${n}_out.txt")
  args=$(cat "./func_tests/pos_${n}_args.txt")
  
  if [[ "$(expr substr $(uname -s) 1 6)" != "CYGWIN" && $(command -v drmemory &> /dev/null) ]]
  then
    status=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- ./app.exe $args ./func_tests/out/pos_${n}_out_file.txt 1>./func_tests/out/pos_${n}_out.txt 2>./func_tests/out/pos_${n}_err.txt)
  elif command -v valgrind &> /dev/null
  then
    status=$(valgrind -q -- ./app.exe $args ./func_tests/out/pos_${n}_out_file.txt 1>./func_tests/out/pos_${n}_out.txt 2>./func_tests/out/pos_${n}_err.txt)
  else
    status=$(./app.exe $args ./func_tests/out/pos_${n}_out_file.txt 1>./func_tests/out/pos_${n}_out.txt 2>./func_tests/out/pos_${n}_err.txt)
  fi
  
  status=$?
  actual=$(cat ./func_tests/out/pos_${n}_out_file.txt)

  if [ $status -ne 0 ]
  then
    echo "Тест #$i: Упал с кодом $status" 
  else
    diff -q "./func_tests/pos_${n}_out.txt" "./func_tests/out/pos_${n}_out_file.txt" >/dev/null
    if [ $? -eq 0 ]
    then
      passed_pos=$(( passed_pos + 1 ))
      echo "Тест #$i: Прошёл."
    else
      echo "Тест #$i: Упал."
      echo "Ожидалось: $expected"
      echo "Вместо: $actual"
    fi
  fi
done

# negative tests counters
declare -i total_neg=$(ls -1q ./func_tests/neg_*_args.txt 2>/dev/null | wc -l)
declare -i passed_neg=0

# test negative cases
printf "\n  Негативные тесты:\n\n"
for ((i = 1; i <= $total_neg; i++))
do
  # gather expected and actual strings
  n=$(printf "%02d" $i)
  #expected=$(cat "./func_tests/neg_${n}_out.txt")
  args=$(cat "./func_tests/neg_${n}_args.txt")
  
  if [[ "$(expr substr $(uname -s) 1 6)" != "CYGWIN" && $(command -v drmemory &> /dev/null) ]]
  then
    res=$(drmemory.exe -batch -ignore_kernel -no_use_stderr -- ./app.exe $args ./func_tests/out/neg_${n}_out_file.txt 1>./func_tests/out/neg_${n}_out.txt 2>./func_tests/out/neg_${n}_err.txt)
  elif command -v valgrind &> /dev/null
  then
    res=$(valgrind -q -- ./app.exe $args ./func_tests/out/neg_${n}_out_file.txt 1>./func_tests/out/neg_${n}_out.txt 2>./func_tests/out/neg_${n}_err.txt)
  else
    res=$(./app.exe $args ./func_tests/out/neg_${n}_out_file.txt 1>./func_tests/out/neg_${n}_out.txt 2>./func_tests/out/neg_${n}_err.txt)
  fi

  status=$?

  if [[ $status == 0 || "$res" != "" ]]
  then
    echo "Тест #$i: Упал с кодом $status" 
  else
    passed_neg=$(( passed_neg + 1 ))
    echo "Тест #$i: Прошёл с кодом $status"
  fi
done

# rm -rf ./.temp_*

echo
echo "Позитивных тестов: $total_pos всего, $passed_pos прошло, $(( total_pos - passed_pos )) упало."
echo "Негативных тестов: $total_neg всего, $passed_neg прошло, $(( total_neg - passed_neg )) упало."

printf "\n  Сбор инормации о покрытии...\n\n"

# move to proper place
for srcf in $(find ./src/ -name *.c)
do
    dir=$(dirname $srcf)
    srcf_base=$(basename $srcf)
    f="${srcf_base%.*}"

    mv ./obj/$f.gcda $dir/$f.gcda
    mv ./obj/$f.gcno $dir/$f.gcno
done

declare -i percentage=0
declare -i amount=$(find ./src -name *.c | wc -l)
for f in $(find ./src -name *.c); do
  mkdir -p ./func_tests/gcov/$(dirname $f)
  base_name=$(basename $f)
  out_file="./func_tests/gcov/src/.$(dirname $f)/$base_name.gcov"
  gcov "$f" -t > $out_file
  percentage_i=`gcov "$f" -n | grep -E -o "[0-9]+" | head -1`
  percentage=$(( percentage + percentage_i / amount ))
done
echo "Покрытие кода тестами: $percentage%"

# clean up everything except *.c.gcov
find ./src -name *.gcno -delete
find ./src -name *.gcda -delete
