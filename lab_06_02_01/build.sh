mkdir -p ./bin
gcc --std=c99 --coverage -pedantic -Wall -Wvla -Werror -O0 $(find . -name '*.c') -o ./bin/app.exe