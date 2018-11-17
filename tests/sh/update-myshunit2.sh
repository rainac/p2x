#! /bin/bash

cp $(which shunit2) myshunit2

patch < myshunit2.patch myshunit2

