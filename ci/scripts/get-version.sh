#! /bin/bash
vn=$(grep AC_INIT configure.ac| sed -e 's/,/, /g' | awk '{ print $2 }' | tr -d '\,\[\]')
echo -n "$vn"

