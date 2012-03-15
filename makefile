OUTPUT=maccoun_assignment8
INPUTS=./src/maccoun_assignment8.cpp
LIBS=-lGL -lglut -lGLU -lIL -lGLEW -lpthread

CC=g++
LIB_PATH=/usr/lib/

all: proj

proj: $(OUTPUT).o
	$(CC) $(OUTPUT).o  -L$(LIB_PATH) $(LIBS) -o $(OUTPUT)
	rm *.o

$(OUTPUT).o: $(INPUTS)
	$(CC) -c $(INPUTS)


clean:
	rm $(OUTPUT)

run:
	./$(OUTPUT)
