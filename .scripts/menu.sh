#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : menu
# @created     : Wednesday Sep 25, 2019 16:57:36 MDT
#
# @description : Menus and guis
######################################################################


menu () {
printf "
  Builder options

  ./compile.sh  help                    # Shows a useful git help utility
  ./compile.sh  update                  # Pulls from git remote and checks for changes
  ./compile.sh  all                     # builds everything from a clean build
  ./compile.sh  messages                # updates message libraries (pulls from sentinet_message_pkg
  ./compile.sh  install_ubuntu_deps     # installs ubuntu dependencies
  ./compile.sh  format                  # formats code
  ./compile.sh  clean                   # cleans build
  ./compile.sh  basic                   # just builds libraries
  "
  echo ""
}

menu2 () {
printf "

  Git Help
  First things first, read the docs. https://git-scm.com/docs
  Also, check the man page

    $ man git

  CREATE A BRANCH
   
    $ git checkout -b <branch name>
    $ git push -u origin <branch name>

  GRAB CHANGES

    $ git pull --rebase
    $ git rebase origin/master
    $ git push origin <branch name>

    # Or, I've added a utility script
    $ ./compile update

  ADD STUFF

    $ git add <stuff>
    $ git commit
    $ git push origin <branch name>

  HELPFUL COMMANDS

    $ git status
    $ git remote -v
    $ git log
    $ git reflog

  "
  echo ""
}

