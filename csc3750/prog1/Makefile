CC = g++
GCCFLAGS = -O2
INCLUDE = -I/usr/include
FILENAME = Main
LDLIBS =   -lGL -lGLU -lglut
LDFLAGS =  -L/usr/lib -L/lib

all: Picture.o Window.o Pixel.o Color.o Matrix.o BasicMatrix.o 
	@$(CC) Main.cpp Picture.o Window.o Pixel.o Matrix.o BasicMatrix.o Color.o BasicObject.o FileIO.o Vertex.o Face.o AffineTransforms.o $(LDLIBS) -o $(FILENAME)

BasicMatrix.o: BasicMatrix.cpp BasicMatrix.h
	@$(CC) -c BasicMatrix.cpp

Matrix.o: BasicMatrix.o Matrix.cpp Matrix.h
	@$(CC) -c Matrix.cpp

AffineTransforms.o: AffineTransforms.h AffineTransforms.cpp Matrix.o
	@$(CC) -c AffineTransforms.cpp

Picture.o: Picture.cpp Picture.h BasicObject.o FileIO.o Vertex.o Face.o AffineTransforms.o
	@$(CC) -c Picture.cpp $(LDLIBS)

Window.o: Window.cpp Window.h
	@$(CC) -c Window.cpp $(LDLIBS)

Pixel.o: Pixel.cpp Pixel.h
	@$(CC) -c Pixel.cpp $(LDLIBS)

Color.o: Color.cpp Color.h
	@$(CC) -c Color.cpp $(LDLIBS)

Vertex.o: Vertex.cpp Vertex.h Color.o Matrix.o
	@$(CC) -c Vertex.cpp

Face.o: Face.cpp Face.h Vertex.o
	@$(CC) -c Face.cpp

BasicObject.o: BasicObject.cpp BasicObject.h Face.o
	@$(CC) -c BasicObject.cpp

FileIO.o: FileIO.cpp
	@$(CC) -c FileIO.cpp
clean:
	rm *.o
	rm Main
