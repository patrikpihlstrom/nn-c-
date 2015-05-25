#!/bin/bash

printf "Host: "
read host

# Retrieve executable and execute said executable.
rsync -arvzSH --include="No-Motivation" --exclude="*" cognizance@$host:~/Projects/tribal/ ~/Projects/tribal/
__GL_SYNC_TO_VBLANK=0 optirun ./No-Motivation

