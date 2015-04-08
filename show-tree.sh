#! /bin/bash

set -x

LD_P2C=examples/configs/default

mypid=$$

src/p2x -p examples/configs/postfix-test < $1 | xsltproc src/xsl/but-root.xsl - > /tmp/tmp$mypid.xml
xml-graph.sh -g src/xml/show.graphotron.xml -o /tmp/tmp$mypid.png /tmp/tmp$mypid.xml
#rm /tmp/tmp$mypid.xml
gwenview /tmp/tmp$mypid.png
#rm /tmp/tmp$mypid.png

