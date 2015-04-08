#! /bin/sh
git log -n 1 | head -1 | awk '{print $2}'
