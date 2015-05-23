#!/bin/bash
# This script will send the source files to the server, tell said server to compile the source files
# , and retrieve the executable file.

echo "Scons option: "
read option

rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ root@188.226.217.13:/root/Projects/tribal/ && ssh root@188.226.217.13 "cd Projects/tribal/; scons $option"

rsync -arvzSH --include="No-Motivation" --exclude="*" root@188.226.217.13:/root/Projects/tribal/ ~/Projects/tribal/ && ~/Projects/tribal/run.sh

