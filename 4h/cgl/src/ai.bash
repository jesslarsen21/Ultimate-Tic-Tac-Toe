#!/bin/bash

while /bin/true; do
    echo "Here we go again."
    UBUNTU_MENUPROXY= python ./uttt_main.py --user lisa --password foobar --ai --ai-level 5 --no-gui;
    echo ""
done
