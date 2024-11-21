#!/bin/bash

echo "Format code using Google style "

for a in applications libraries; do
    #echo $a
    pushd $a > /dev/null
    for b in *; do
        if [ -d $b ]
        then
            #echo $b
            pushd $b > /dev/null
            for c in *; do
                if [ -d $c ]
                then
                    #echo $c
                    pushd $c > /dev/null
                    for f in *.{h,cpp}; do
                        if [ -f $f ]
                        then
                            echo -n "Formatting $a/$b/$c/$f ... "
                            clang-format-3.9 -style=file -i $f
                            echo Formatted
                        fi
                    done
                    rm *.h-* *.cpp-*
                    popd > /dev/null
                fi
            done
            popd > /dev/null
        fi
    done
    popd > /dev/null
done
    