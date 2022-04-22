#! /bin/bash
comid=$(git rev-parse HEAD)
if [[ "$?" = "0" ]]; then
    comid=${comid:0:8}
else
    comid=$(cat vcs-version.txt)
fi
echo -n "${comid}"
