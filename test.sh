#!/bin/bash

#printf "Host: "
#read host

host="Luke"

rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ cognizance@$host:~/Projects/tribal/ && ssh cognizance@$host "cd Projects/tribal/; scons"

printf "Retrieving the executable from Luke and running.\n"
rsync -arvzSH --include="No-Motivation" --exclude="*" cognizance@$host:~/Projects/tribal/ ~/Projects/tribal/
__GL_SYNC_TO_VBLANK=0 ./No-Motivation

