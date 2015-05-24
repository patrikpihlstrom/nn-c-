#!/bin/bash

printf "Scons option: "
read option

# Send src and compile.
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ cognizance@188.226.217.13:~/Projects/tribal/ && ssh cognizance@188.226.217.13 "cd Projects/tribal/; scons $option"

