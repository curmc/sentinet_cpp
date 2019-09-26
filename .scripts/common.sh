#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : common
# @created     : Wednesday Sep 25, 2019 16:59:08 MDT
#
# @description : Common functions
######################################################################

SUBMODULE_DIR="$PWD/third_party/sentinet_message_pkg"
ERROR_NO_ADD="\nIssue the commands \n\n \$ git add <files> \n\nand \n\n \$ git commit \n\nto proceed\n\n"
INVALID_RESP="\nInalid response\n"
ERROR_NO_ADD_SIG=1
INVALID_RESP_SIG=2

_exit () {
  printf "$1"
  if [ -z $2 ]; then
    exit $2
  else
    exit 0
  fi
}
