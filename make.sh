#!/usr/bin/env bash

set -e # fail on unhandled error
set -u # fail on undefined variable
#set -x # debug

declare -a sysTools=( "make" )

function commandExists()
{
  type "$1" &> /dev/null
}

function usage()
{
  echo "$(basename "$0") [gcc|clang|cross-android] (default is gcc)"
  exit 1
}

function build_cross_android()
{
  local cpu cpuBaseDir
  if [[ -z ${NDK+x} ]]; then
    # Search in $PATH
    if [[ $(which ndk-build) != "" ]]; then
      NDK=$(dirname "$(which ndk-build)")
    else
      echo "[-] Could not detect Android NDK dir"
      exit 1
    fi
  fi

  ndk-build clean
  ndk-build || {
    echo "[-] android build failed"
    exit 1
  }

  find libs -type d -mindepth 1 -maxdepth 1 | while read -r cpuBaseDir
  do
    cpu=$(basename "$cpuBaseDir")
    cp libs/"$cpu"/dexRepair bin/dexRepair-"$cpu"
  done
}

function build()
{
  local compiler="$1"

  make clean -C src || {
    echo "[-] make clean failed"
    exit 1
  }

  CC=$compiler make -C src || {
    echo "[-] clang build failed"
    exit 1
  }
}

# Check that common system tools exist
for i in "${sysTools[@]}"
do
  if ! commandExists "$i"; then
    echo "[-] '$i' command not found"
    exit 1
  fi
done

if [ $# -gt 1 ]; then
  echo "[-] Invalid args"
  exit 1
fi

if [ $# -eq 0 ]; then
  target=""
else
  target="$1"
fi

case "$target" in
  "") build "gcc";;
  "gcc") build "gcc";;
  "clang") build "clang";;
  "cross-android") build_cross_android;;
  *) usage;;
esac

exit 0
