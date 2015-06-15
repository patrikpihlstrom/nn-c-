#!/bin/bash

printf "Building on Vader with scons option -j8."
rsync -arvzSH --include="*.cpp" --include="*.hpp" --include="SConstruct" --include="*.lua" --include="*/" --exclude="*" ~/Projects/tribal/ cognizance@Vader:~/Projects/tribal/ && ssh cognizance@Vader "cd Projects/tribal/; scons -j8"

printf "Retrieving the executable from Vader and running."
rsync -arvzSH --include="No-Motivation" --exclude="*" cognizance@Vader:~/Projects/tribal/ ~/Projects/tribal/
__GL_SYNC_TO_VBLANK=0 optirun ./No-Motivation

