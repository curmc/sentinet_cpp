#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : git_init
# @created     : Wednesday Sep 25, 2019 16:58:13 MDT
#
# @description : A git initializer script
######################################################################


git_init () {
  git submodule init
  git submodule update
}

