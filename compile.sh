#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : build
# @created     : Friday Sep 20, 2019 20:07:14 MDT
#
# @description : Build script
######################################################################

source "$PWD/.scripts/scan_option.sh"
source "$PWD/.scripts/menu.sh"

main () {
  if [ $# -gt 0 ]; then
    scan_option $1
  else
    menu
  fi
}

main $@
