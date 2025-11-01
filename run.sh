#!/bin/bash

cmake -B build
cmake --build build
./build/bin/main

## How to setup a file to let lsp know your imports
#In `cmake` file you need to add this 
#```sh
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#```
#This will say to make a output file that like  package.json
#Lsp can use this to tell where the imports are 
#```sh
#cmake -B build
#ln -s build/compile_commands.json


#Editing multiple line in NVIM
#You can use the follow commands
#s/^/{put-something}   /
