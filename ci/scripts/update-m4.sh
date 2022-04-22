#! /bin/bash

sed -e '/#serial/ d' m4/*.m4 > acinclude.m4
