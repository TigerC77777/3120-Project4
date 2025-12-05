CFLAGS= -Wall -g
FILES= unionFindController.cpp unionFindView.cpp unionFindModel.cpp
TARGET= proj3.exe
EXTRA= -IC:\mingw\include\SDL3 -LC:\mingw\lib\ -lmingw32 -lSDL3

# These variables can be set on the command line, and define the width and height of the maze
width=10
height=10

build:
	g++ $(FILES) -o $(TARGET) $(EXTRA)
run:
	.\$(TARGET) $(width) $(height)
clean:
	rm -f $(TARGET)