#!/bin/sh

gcc -g src/main.c src/turan_choks.c src/upper_graphics.c -Isrc/external/glad/include -L$(brew --prefix)/lib -I$(brew --prefix)/include src/external/glad/src/gl.c -lSDL2 -lwebp -lwebpdemux -Wpointer-sign -o choks