#!/usr/bin/env sh

source "$PWD/.scripts/scan_option.sh"
hello there
source "$PWD/.scripts/menu.sh"

main () {
  if [ $# -gt 0 ]; then
    scan_option $1
  else
    menu
  fi
}

main $@
