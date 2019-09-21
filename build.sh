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

  ./build.sh  all                     # builds everything from a clean build
  ./build.sh  messages                # updates message libraries (pulls from sentinet_message_pkg
  ./build.sh  install_ubuntu_deps     # installs ubuntu dependencies
  ./build.sh  format                  # formats code
  ./build.sh  clean                   # cleans build
  ./build.sh  basic                   # just builds libraries
  "
  echo ""
}


scan_option () {
  while test $# -gt 0; do
    case "$1" in
    all)
      make all
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
      make
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
