#!/usr/bin/env sh

######################################################################
# @author      : theo (theo.j.lincke@gmail.com)
# @file        : linker
# @created     : Tuesday Oct 15, 2019 11:16:51 MDT
#
# @description : Links scripts from sentinet to lib
######################################################################

PREFIX="/opt/sentinet_cpp"

link_includes () {
  if [ -d "/usr/include/scpp" ]; then
    echo "Already linked"
    return
  fi

  if [ -d "$PREFIX/core" ]; then
    sudo ln -sf $PREFIX/core/include/scpp /usr/include/scpp
  else
    echo "You haven't installed sentinet yet"
  fi
}

link_lib () {
  if [ -f /usr/lib/$1 ]; then
    echo "Already linked $1"
    return
  fi

  if [ -f "$PREFIX/core/lib/$1" ]; then
    sudo ln -sf $PREFIX/core/lib/$1 /usr/lib/$1
  else
    echo "You don't have $1 installed"
  fi
}

remove_all () {
  sudo rm -rf /usr/lib/libmessages.a
  sudo rm -rf /usr/lib/libkernel.a
  sudo rm -rf /usr/lib/libcontrol.a
  sudo rm -rf /opt/sentinet_cpp
}

link_libs () {
  link_lib "libmessages.a"
  link_lib "libcontrol.a"
  link_lib "libkernel.a"
}

