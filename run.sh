#!/bin/bash

# Retrieve executable and execute said executable.
rsync -arvzSH --include="No-Motivation" --exclude="*" root@188.226.217.13:/root/Projects/tribal/ ~/Projects/tribal/ && __GL_SYNC_TO_VBLANK=0 optirun ./No-Motivation

