#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./main "$input" > tmp.s
    gcc tmp.s -o tmp
    ./tmp

    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

try 0 0
try 50 50

echo OK
