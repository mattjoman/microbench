#!/bin/bash

echo $1

make clear

if [[ "$1" = "debug" ]]; then
    echo "Making debugging build";
    make BUILD_CFLAGS="-Iinclude -O0 -Wall -Wextra -std=gnu11 -g";
elif [[ "$1" = "prod" ]]; then
    echo "Making production build";
    make BUILD_CFLAGS="-O3 -std=gnu11";
else
    echo "Making debugging build";
    make BUILD_CFLAGS="-Iinclude -O0 -Wall -Wextra -std=gnu11 -g";
fi;
