#!/bin/bash
# Skrypt dodający zadaną ilość samochodów min 1 max 10
cd src
git clone https://github.com/eatrunner/anro1.git
cd anro1
git checkout antURDF
cd ../..
source /opt/ros/jade/setup.bash
catkin_make
