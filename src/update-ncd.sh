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

which=$1

update_token() {
    $GENNC -i -n Token xml/token.ncd.xml
}

update_logger() {
    $GENNC -n LS -i -I xml/logger.ncd.xml
}

update_modes() {
    $GENNC -f xml/modes.ncd.xml
}

update_assoc() {
    $GENNC -f xml/assoc.ncd.xml
}

update_scanners() {
    $GENNC -f xml/scanners.ncd.xml
}

update_output_modes() {
    $GENNC -f xml/output-modes.ncd.xml
}

if [[ -z "$which" ]]; then

    update_token
    update_logger
    update_modes
    update_assoc
    update_scanners
    update_output_modes

else

    case $which in
	(*token*)
	    update_token
	    ;;
	(*logger*)
	    update_logger
	    ;;
	(*output_modes*)
	    update_output_modes
	    ;;
	(*modes*)
	    update_modes
	    ;;
	(*assoc*)
	    update_assoc
	    ;;
	(*scanners*)
	    update_scanners
	    ;;
    esac    

fi
