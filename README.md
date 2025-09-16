# Oreon System Manager (Qt)
**An all in one system manager for managing DNF packages, repositories, containers with distrobox and docker, and pro audio config all in a single Qt interface.**

NOTE: THIS PROJECT IS STILL IN EARLY DEVELOPMENT, PLEASE REPORT BUGS IF YOU FIND ANY.

## Installation
If you are on Oreon 10 (Build 2507) or later, this comes out of the box. On Oreon 10 (Build 2504.4) or older, run `sudo dnf install oreon-system-manager` to install.

## Manual Compilation
Install needed dependencies with `sudo dnf install qt6-qtbase-devel qt6-qttools-devel cmake gcc-c++` then run `mkdir build && cd build && cmake .. && make` and finally start the app with `./oreon-system-manager`