all:
	g++ src/*.cpp -o build/launcher.exe -I./include -L./lib -llua -lraylib -lwinmm -lgdi32