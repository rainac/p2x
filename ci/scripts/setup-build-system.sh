#! /bin/bash

mydir=$(dirname $BASH_SOURCE)

sudo apt-get install $(cat $mydir/../debian-packages.txt)
