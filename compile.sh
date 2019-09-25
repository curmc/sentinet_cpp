#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : build
# @created     : Friday Sep 20, 2019 20:07:14 MDT
#
# @description : Build script
######################################################################


SUBMODULE_DIR="$PWD/third_party/sentinet_message_pkg"
ERROR_NO_ADD="\nIssue the commands \n\n \$ git add <files> \n\nand \n\n \$ git commit \n\nto proceed\n\n"
INVALID_RESP="\nInalid response\n"
ERROR_NO_ADD_SIG=1
INVALID_RESP_SIG=2

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


_exit () {
  printf "$1"
  if [ -z $2 ]; then
    exit $2
  else
    exit 0
  fi
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

git_rebase () {
  # Fetch sources from master
  echo "====================================="
  git fetch origin master && echo "Fetched from remote updating sources now" || _exit "$ERROR_NO_ADD" $ERROR_NO_ADD_SIG
  
  # Pull sources and rebase onto branch
  git pull --rebase && echo "Rebased onto branch, continue? [Y/n]" || _exit "$ERROR_NO_ADD" $ERROR_NO_ADD_SIG

  # Read a response
  read resp && echo "You entered $resp" || _exit "$INVALID_RESP"

  # Rebase if yes
  if [ $resp == "y" ]; then
    echo "====================================="
    git rebase origin/master && echo "Finished rebasing show status? [Y/n]"
    read resp2 
    if [ $resp2 == "y" ]; then
      git reflog 
    fi
  else
    echo "Ok"
  fi
  echo "====================================="
  echo "Checking for submodule status"
  
  if [ ! -f "$SUBMODULE_DIR" ]; then
    git_init && git_update
  fi

  git submodule update --remote --rebase
  echo "All up to date! Should these changes be pushed to the remote? [Y/n]" && read resp3
  if [ $resp3 == "y" ]; then
    git push origin $(git symbolic-ref HEAD 2>/dev/null | cut -d"/" -f 3)
  fi
  echo "====================================="
}

git_init () {
  git submodule init
  git submodule update
}

git_update () {
  git fetch
  git submodule update --remote --rebase
}

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
