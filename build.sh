#!/bin/bash

echo "Scons option: "
read option

# Send src and compile.
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ root@188.226.217.13:/root/Projects/tribal/ && ssh root@188.226.217.13 "cd Projects/tribal/; scons $option"

