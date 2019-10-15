#!/bin/bash

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
