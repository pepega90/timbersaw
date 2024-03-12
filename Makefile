all: compile run
compile:
	g++ main.cpp -o main.exe -I ./deps/include/ -L ./deps/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32
run:
	.\main.exe
web:
	emcc -std=c++17 -o ./build/index.html main.cpp -Os -Wall -I ./include/ -L ./lib/ -lraylib -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH -DPLATFORM_WEB --preload-file texture --preload-file snd
