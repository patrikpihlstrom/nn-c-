#!/bin/bash

printf "Host: "
read host

printf "Scons option: "
read option

# Send src and compile.
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ cognizance@$host:~/Projects/tribal/ && ssh cognizance@$host "cd Projects/tribal/; scons $option"

