#!/bin/bash

# Retrieve executable and execute said executable.
rsync -arvzSH --include="No-Motivation" --exclude="*" cognizance@188.226.217.13:~/Projects/tribal/ ~/Projects/tribal/ && __GL_SYNC_TO_VBLANK=0 optirun ./No-Motivation

