#! /bin/bash

apt-get -y install git g++ gcc make automake shunit2 xmlstarlet xsltproc flex libcppunit-dev zsh octave trang npm

npm install -g mocha
npm install -g tmp
npm install -g most-xml

sudo ln -sf /usr/bin/nodejs /usr/local/bin/node
