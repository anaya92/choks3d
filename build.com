#!/bin/sh

gcc -O2 src/main.c src/turan_choks.c src/upper_graphics.c src/lolita.c src/software_clustering.c -Isrc/external/glad/include src/external/glad/src/gl.c -lSDL2 -lwebp -lwebpdemux -Wpointer-sign -o choks