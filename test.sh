#!/bin/bash

printf "Building on Luke."
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ cognizance@luke:~/Projects/tribal/ && ssh cognizance@luke "cd Projects/tribal/; scons"

printf "Retrieving the executable from Luke and running.\n"
rsync -arvzSH --include="No-Motivation" --exclude="*" cognizance@luke:~/Projects/tribal/ ~/Projects/tribal/
__GL_SYNC_TO_VBLANK=0 optirun ./No-Motivation

