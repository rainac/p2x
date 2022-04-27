#! /bin/bash

mydir=$(dirname $BASH_SOURCE)
top_srcdir=$mydir/../..


for i in $top_srcdir/docker/debian-*; do
    cp -v $top_srcdir/ci/debian-packages.txt $i
    cat $i/Dockerfile - >$i/Dockerfile.build <<EOF

USER root

COPY debian-packages.txt .

RUN apt-get -y install \$(cat debian-packages.txt) debhelper-compat eatmydata git-buildpackage

USER devel

EOF
done

