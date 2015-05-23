#!/bin/bash

echo "Scons option: "
read option

# Send src and compile.
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ root@188.226.217.13:/root/Projects/tribal/ && ssh root@188.226.217.13 "cd Projects/tribal/; scons $option"

# Retrieve executable and execute with run.sh.
rsync -arvzSH --include="No-Motivation" --exclude="*" root@188.226.217.13:/root/Projects/tribal/ ~/Projects/tribal/ && ~/Projects/tribal/run.sh

