#!/usr/bin/zsh
for i in {0..10000}
do
    ./main < input.txt > /dev/null
done