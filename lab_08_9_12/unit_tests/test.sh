#!/bin/bash

valgrind -q ./unit_tests.exe

mkdir -p ./unit_tests/gcov
printf "\n  Сбор инормации о покрытии...\n\n"

# move to proper place
for srcf in $(find ./ -name *.c)
do
    dir=$(dirname $srcf)
    srcf_base=$(basename $srcf)
    f="${srcf_base%.*}"

    if [ -f ./$f.gcda ]
    then
        mv ./$f.gcda $dir/$f.gcda >/dev/null
        mv ./$f.gcno $dir/$f.gcno >/dev/null
    fi
done

declare -i percentage=0
declare -i amount=0
for f in $(find . -name *.c); do
    fbase=$(basename $f)
    if [ -f $(dirname $f)/${fbase%.*}.gcda ]
    then
        mkdir -p ./unit_tests/gcov/src/.$(dirname $f)
        base_name=$(basename $f)
        out_file="./unit_tests/gcov/src/.$(dirname $f)/$base_name.gcov"
        gcov "$f" -t > $out_file
        percentage_i=`gcov "$f" -n | grep -E -o "[0-9]+" | head -1`
        percentage=$(( percentage + percentage_i ))
        amount=$(( amount + 1 ))
    fi
done
if [ amount != 0 ]
then
    percentage=$(( percentage / amount ))
fi
echo "Покрытие кода тестами: $percentage%"

# clean up everything except *.c.gcov
find . -name *.gcno -delete
find . -name *.gcda -delete
