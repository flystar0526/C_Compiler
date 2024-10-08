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

try 0 "0"
try 50 "50"
try 21 "5+20-4"
try 25 "5+10+10"
try 41 " 12 + 34 - 5 "
try 50 "   50 - 20 + 20"
try 47 "5 + 6 * 7"
try 15 "5 * (9 - 6)"
try 4 "(3 + 5) / 2"
try 10 "-10 + 20"
try 8 "-(-3 - +5)"
try 0 "0 == 1"
try 1 "42 == 42"
try 1 "0 != 1"
try 0 "42 != 42"
try 1 "0 < 1"
try 0 "1 < 1"
try 0 "2 < 1"
try 1 "0 <= 1"
try 1 "1 <= 1"
try 0 "2 <= 1"
try 1 "1 > 0"
try 0 "1 > 1"
try 0 "1 > 2"
try 1 "1 >= 0"
try 1 "1 >= 1"
try 0 "1 >= 2"

echo OK
