#! /bin/bash

# This script updates the generated enumeration code files from the
# definitions in xml/*.ncd.xml using the gennc tool
# (http://www.nongnu.org/named-constant/)
#
# with a local installation of gennc, run like this
#
# GENNC_HOME=$PREFIX GENNC=$PREFIX/bin/gennc ./update-ncd.sh

#set -x
GENNC=${GENNC:-gennc}

$GENNC -n Token xml/token.ncd.xml
$GENNC -n LS -i xml/logger.ncd.xml
$GENNC -f xml/modes.ncd.xml
$GENNC -f xml/assoc.ncd.xml
$GENNC -f xml/scanners.ncd.xml
$GENNC -f xml/output-modes.ncd.xml
