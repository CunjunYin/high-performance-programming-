# CITS3402 High Performance Computing
parallel programming
for mac os that unable to run openMP do folowing steps:

1: install brew by terminal CMD:/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

2: install gcc6 CMD: brew reinstall gcc6 --without-multilib

3 open MP lib CMD: brew install llvm

4: gcc-6 -fopenmp -o progm progm.c

VM Linux GCC install process:

1.sudo dpkg --configure -a

2. sudo apt install gcc
