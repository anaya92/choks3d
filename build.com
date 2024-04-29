#!/bin/sh

gcc -g src/main.c src/turan_choks.c src/upper_graphics.c src/lolita.c src/software_clustering.c -Isrc/external/glad/include src/external/glad/src/gl.c -lm -lSDL2 -lwebp -lwebpdemux -Wpointer-sign -o choks