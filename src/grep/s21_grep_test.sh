#!/bin/bash

ok=0
fails=0
counter=0
check=""
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)"
COLOR_RED="$(tput setaf 1)"

declare -a test_files=(
"-efor s21_grep.c s21_grep.h Makefile FLAGS"
"-efor s21_grep.c FLAGS"
"EOF file_does_not_exist.txt FLAGS"
"-fpattern.txt s21_grep.c FLAGS"
"-emain -f pattern.txt s21_grep.c s21_grep.h Makefile FLAGS"
"-e for -e ^#include s21_grep.c s21_grep.h Makefile FLAGS"
)

declare -a more_tests=(
"-v test_1_grep.txt -e sad"
"-l for test_1_grep.txt test_2_grep.txt"
"-o -e int test_4_grep.txt"
"-c -e . test_1_grep.txt -e '.'"
"-l for file_does_not_exist.txt test_2_grep.txt"
"-n for test_1_grep.txt test_2_grep.txt"
"-n for test_1_grep.txt"
"-n -e ^\} test_1_grep.txt"
"-c -e /\ test_1_grep.txt"
"-ivnh = test_1_grep.txt test_2_grep.txt"
"-eabc test_1_grep.txt test_2_grep.txt"
"-ivclnhs -eint s21_grep.c"
"-ivclnhso -eint s21_grep.c"
)

compare()
{
    parameters=$(echo $@ | sed "s/FLAGS/$flags/")
    ./s21_grep $parameters > s21_grep_log.txt
    grep $parameters > grep_log.txt
    check="$(diff -s s21_grep_log.txt grep_log.txt)"
    if [ "$check" == "Files s21_grep_log.txt and grep_log.txt are identical" ]
    then
        printf '%s%s%s%s%s' '[' $COLOR_GREEN '  OK  ' $COLOR_REST '] '
        echo "./s21_cat $parameters"
        ((ok++))
    else
        printf '%s%s%s%s%s' '[' $COLOR_RED ' FAIL ' $COLOR_REST '] '
        echo "./s21_cat $parameters"
        ((fails++))
    fi
    ((counter++))
    rm s21_grep_log.txt grep_log.txt
}

for i in "${more_tests[@]}"
do
    flags="-"
    compare $i
done

for j in v c l n h o
do
    for i in "${test_files[@]}"
    do
        flags="-$j"
        compare $i
    done
done

for j in v c l n h o
do
    for k in v c l n h o
    do
        if [ $j != $k ]
        then
            for i in "${test_files[@]}"
            do
                flags="-$j -$k"
                compare $i
            done
        fi
    done
done

for j in v c l n h o
do
    for k in v c l n h o
    do
        for l in v c l n h o
        do
            if [ $j != $k ] && [ $k != $l ] && [ $j != $l ]
            then
                for i in "${test_files[@]}"
                do
                    flags="-$j -$k -$l"
                    compare $i
                done
            fi
        done
    done
done

printf '%s%s%s%s%s%s' '[' $COLOR_GREEN '  OK:  ' $ok $COLOR_REST '  ]   '
printf '%s%s%s%s%s%s' '[' $COLOR_RED '  FAILS:  ' $fails $COLOR_REST '  ]   '
printf '%s%s%s%s\n' '[' '  TOTAL:  ' $counter '  ]'
