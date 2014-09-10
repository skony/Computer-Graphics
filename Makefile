INCLUDE = -I/usr/include/
LIBDIR  = -L/usr/X11R6/lib 

LIBS=-lGL -lglut -lGLEW -lGLU -lX11 -lXi -lXmu -lm
CC=g++

SOURCES=main_file.cpp lib/EasyBMP.cpp common/Model_OBJ.cpp common/lesson13.cpp common/DirectoryMonitor.cpp
HEADERS=
OBJECTS=$(SOURCES:.cpp=.o)

all: main_file

main_file: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CC) -std=gnu++0x -c -o  $@ $(LIBDIR) $< $(LIBRARIES)  
	
clean:
	-rm -rf *.o main_file
