#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : scan_option
# @created     : Wednesday Sep 25, 2019 16:58:31 MDT
#
# @description : Scans the command line for options
######################################################################

source "$PWD/.scripts/git_init.sh"
source "$PWD/.scripts/git_rebase.sh"
source "$PWD/.scripts/git_update.sh"
source "$PWD/.scripts/linker.sh"

scan_option () {
  while test $# -gt 0; do
    case "$1" in
    help)
      menu2
      shift
      ;;
    init)
      git_update
      shift
      ;;
    all)
      make all
      printf "\n\n>>>  Binary files in ./build/bin\n\n>>>  Library files in ./build/lib\n\n"
      shift
      ;;
    messages)
      make msgs_update
      shift
      ;;
    install_ubuntu_deps)
      make install-deps
      shift
      ;;
    format)
      make format
      shift
      ;;
    clean)
      make clean-all
      shift
      ;;
    basic)
      make basic
      shift
      ;;
    update)
      git_rebase
      shift
      ;;
    install)
      make install-deps
      make && sudo make install
      link_includes
      link_libs
      shift
      ;;
    uninstall)
      remove_all 
      shift
      ;;
    *)
      break
      ;;
  esac
done
}
