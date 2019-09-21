#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : build
# @created     : Friday Sep 20, 2019 20:07:14 MDT
#
# @description : Build script
######################################################################


menu () {
printf "
  Builder options

  ./compile.sh  all                     # builds everything from a clean build
  ./compile.sh  messages                # updates message libraries (pulls from sentinet_message_pkg
  ./compile.sh  install_ubuntu_deps     # installs ubuntu dependencies
  ./compile.sh  format                  # formats code
  ./compile.sh  clean                   # cleans build
  ./compile.sh  basic                   # just builds libraries
  "
  echo ""
}


scan_option () {
  while test $# -gt 0; do
    case "$1" in
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
    *)
      break
      ;;
  esac
done
}


main () {
  if [ $# -gt 0 ]; then
    scan_option $1
  else
    menu
  fi
}

main $@
