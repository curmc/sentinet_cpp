# Installation

Linux support only at the moment

# Build instructions

### Installing dependencies
Note - I need users to test if this installs everything, please contact Theo Lincke (theo.j.lincke@gmail.com) for any dependency issues or ping Theo Lincke on slack
```bash
$ sudo apt install cmake make curl libcurl4-gnutls-dev autoconf automake libtool g++ unzip libzmq3-dev
# or just
$ make install-deps # in SentiNet home dir 
```
  
## To build
``` bash 
$ git clone https://github.com/tlincke125/sentinet_cpp.git sentinet_cpp
$ cd sentinet_cpp
$ make #Note at this early of production, there are bound to be errors
$ make install
# installs to /opt/sentinet_cpp/core/ (lib, bin, include)
# It's a good idea to create a symlink to your libraries in this location to your development environment

# Some other targets

$ make install-deps # installs UBUNTU or ARCH dependencies (using apt)
$ make format # formats code
$ make clean-all # removes build
$ make validate # run unit tests
```

# Running Sentinet and Kermit Kernel
## The Kernel

Unless you are testing serial communication, --debug on should be used
Verbose should generally always be used as well
```bash
$ kermit_kernel --verbose --debug on
```
Creates a new instance of kermit kernel listening on curmt channels.

```bash
$ kermit_proxy --command --cmd_vel --time -1
```
Creates a new instance of a proxy listening on the command and cmd_vel topics, time = -1 runs the proxy forever, specify a time in seconds (ex --time 10 runs proxy for 10 seconds)

Both can be quit with a simple Ctrl+C

Both of these should theoretically be run at the same time. Without proxy, other forms will not be able to pass messages to the kernele







# How to checkout / create / merge a branch

This project is restricted on master and release. You will not be able to commit to these branches. In order to make changes to the project, create a new branch

```bash
  $ git checkout -b my_new_branch
  $ git push -u origin my_new_branch

  ... Do stuff - add changes

  $ git add <specific files>
  $ git status
  $ git commit -m "my_new_branch: added changes"
  $ git push
```

## Updating your branch - rebase it, don't merge 
Every time you make changes, you need to rebase from master. To see why, remember master is recieving new changes all the time. If you push with an old master, this might cause merge conflicts
``` bash
  $ git fetch # not necessary, but I'm showing this here because it's useful to update your branches
  $ git pull --rebase
  $ git rebase origin/master
```
