#!/bin/bash
set +xe


gcc  -Iheaders ./src/enw.c ./src/image_manager.c ./src/ui.c ./src/event_handler.c -Wextra -Wall -lraylib -lGL -lm -ldl -lrt -lX11 -o circlePaper
