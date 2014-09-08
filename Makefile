LIBS=-lGL -lglut -lGLEW -lGLU 
CC=g++

SOURCES=main_file.cpp lib/EasyBMP.cpp common/Model_OBJ.cpp
HEADERS=
OBJECTS=$(SOURCES:.cpp=.o)

all: main_file

main_file: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(CC) -c -std=gnu++0x $< -o $@
	
clean:
	-rm -rf *.o main_file
