all:
	g++ -I src/include -L src/lib -o Johnny_English src/source-code/** -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -Og

d:
	g++ -I src/include -L src/lib -o Johnny_English src/source-code/** -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -Og
	./Johnny_English.exe