#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : git_update
# @created     : Wednesday Sep 25, 2019 16:58:51 MDT
#
# @description : Update git
######################################################################


git_update () {
  git fetch
  git submodule update
}

