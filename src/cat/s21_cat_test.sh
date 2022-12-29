#!/bin/bash

counter=0
check=""
ok=0
fails=0
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)"
COLOR_RED="$(tput setaf 1)"

declare -a test_files=(
"FLAGS test_cat.txt"
)

declare -a more_tests=(
"-b -e -n -s -t -v test_cat_more.txt"
"-n test_cat_more.txt"
"no_file.txt"
"-n -b test_cat_more.txt"
"test_cat_more.txt -n"
"-s test_cat_more.txt"
"-t test_cat_more.txt"
"-n test_cat_more.txt"
"-n test_cat_more.txt test_cat.txt"
"-v test_.txt"
)
compare()
{
    parameters=$(echo $@ | sed "s/FLAGS/$flags/")
    ./s21_cat $parameters > s21_log.txt
    cat $parameters > cat_log.txt
    check="$(diff -s s21_log.txt cat_log.txt)"
    if [ "$check" == "Files s21_log.txt and cat_log.txt are identical" ]
    then
        printf '%s%s%s%s%s%s' '[' $COLOR_GREEN '  OK  ' $COLOR_REST '] '
        echo "./s21_cat $parameters"
        ((ok++))
    else
        printf '%s%s%s%s%s%s' '[' $COLOR_RED ' FAIL ' $COLOR_REST '] '
        echo "./s21_cat $parameters"
        ((fails++))
    fi
    ((counter++))
    rm s21_log.txt cat_log.txt
}

for i in "${more_tests[@]}"
do
    flags="-"
    compare $i
done

for j in b e n s t v
do
    for i in "${test_files[@]}"
    do
        flags="-$j"
        compare $i
    done
done

for j in b e n s t v
do
    for k in b e n s t v
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

for j in b e n s t v
do
    for k in b e n s t v
    do
        for l in b e n s t v
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

for j in b e n s t v
do
    for k in b e n s t v
    do
        for l in b e n s t v
        do
            for m in b e n s t v
            do
                if [ $j != $k ] && [ $k != $l ] \
                && [ $j != $l ] && [ $j != $m ] \
                && [ $k != $m ] && [ $l != $m ]
                then
                    for i in "${test_files[@]}"
                    do
                        flags="-$j -$k -$l -$m"
                        compare $i
                    done
                fi
            done
        done
    done
done

printf '%s%s%s%s%s%s' '[' $COLOR_GREEN '  OK:  ' $ok $COLOR_REST '  ]   '
printf '%s%s%s%s%s%s' '[' $COLOR_RED '  FAILS:  ' $fails $COLOR_REST '  ]   '
printf '%s%s%s%s\n' '[' '  TOTAL:  ' $counter '  ]'
