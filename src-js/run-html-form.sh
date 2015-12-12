#! /bin/bash
mydir=$(readlink -f $(dirname $BASH_SOURCE))
firefox file://$mydir/test-inter.html
