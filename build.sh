#!/bin/bash
set +xe


gcc  enw.c image_manager.c ui.c event_handler.c -Wextra -Wall -lraylib -lGL -lm -ldl -lrt -lX11 -o circlePaper
