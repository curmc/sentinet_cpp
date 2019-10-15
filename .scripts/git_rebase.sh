#!/usr/bin/env sh

######################################################################
# @author      : theo (theo@$HOSTNAME)
# @file        : git_rebase
# @created     : Wednesday Sep 25, 2019 16:58:04 MDT
#
# @description : Git rebase script
######################################################################

source "$PWD/.scripts/common.sh"
source "$PWD/.scripts/git_update.sh"
source "$PWD/.scripts/git_init.sh"

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

  git submodule update
  echo "All up to date! Should these changes be pushed to the remote? [Y/n]" && read resp3
  if [ $resp3 == "y" ]; then
    git push origin $(git symbolic-ref HEAD 2>/dev/null | cut -d"/" -f 3)
  fi
  echo "====================================="
}

