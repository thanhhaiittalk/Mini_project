#!/bin/bash

echo "Time build: $(date '+%Y-%m-%d %H:%M')"
echo "User: $USER"

# Get the current working directory
CRRNT_DIR=$PWD

# Remove the last directory component
PRJ_PATH="${CRRNT_DIR%/*}"

echo "$PRJ_PATH"

echo "Choose build option: 1 - Release  |  2 - Debug"
read MY_OPTION
case $MY_OPTION in
1)
    echo "Release option"
    ;;
2)
    echo "Debug option"
    ;;
*)
    # Invalid option
    echo "Not a valid option. Please choose 1 or 2"
    exit 0
    ;;
esac

mkdir -p $PRJ_PATH/build/
cd $PRJ_PATH/build/

# Clean existing files
if [ "$(ls -A .)" ]; then
  echo "build is not empty. Clean build"
  rm -r *
fi

# export CC=$PRJ_PATH/toolchain/cc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
# export CXX=$PRJ_PATH/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
# Perform make
if [ $MY_OPTION == 1 ]
then
        cmake -DCMAKE_BUILD_TYPE=Release $PRJ_PATH
else
        cmake -DCMAKE_BUILD_TYPE=Debug $PRJ_PATH
fi
make

echo ::::----------------------------::::
echo ::::-----------Finish-----------::::
echo ::::----------------------------::::
