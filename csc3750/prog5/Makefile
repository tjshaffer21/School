CC = g++
INCLUDE = -I/usr/include
FLAGS = -g -O2 -Wall
FILENAME = Main
LDLIBS =   -lglut -lGL -lGLU
LDFLAGS =  -L/usr/lib -L/lib

all: Main.cpp Picture.o Window.o Pixel.o Matrix.o BasicMatrix.o Color.o Vertex.o AffineTransforms.o DeleteObject.o BasicObject.o InstanceObject.o Face.o FileIO.o Scene.o Node.o Vector.o TransformNode.o
	@$(CC) Main.cpp Picture.o Window.o Pixel.o Matrix.o BasicMatrix.o Color.o Vertex.o Vector.o AffineTransforms.o DeleteObject.o BasicObject.o Node.o InstanceObject.o TransformNode.o Face.o Scene.o FileIO.o $(LDLIBS) -o $(FILENAME) $(FLAGS)

FileIO.o:
	@$(CC) -c FileIO.cpp $(FLAGS)
BasicMatrix.o:
	@$(CC) -c BasicMatrix.cpp $(FLAGS)

Matrix.o:
	@$(CC) -c Matrix.cpp $(FLAGS)

Pixel.o:
	@$(CC) -c Pixel.cpp $(FLAGS)

Color.o:
	@$(CC) -c Color.cpp $(FLAGS)

Vertex.o:
	@$(CC) -c Vertex.cpp $(FLAGS)

Vector.o:
	@$(CC) -c Vector.cpp $(FLAGS)

Face.o:
	@$(CC) -c Face.cpp $(FLAGS)

AffineTransforms.o:
	@$(CC) -c AffineTransforms.cpp $(FLAGS)

DeleteObject.o:
	@$(CC) -c DeleteObject.cpp $(FLAGS)

Node.o:
	@$(CC) -c Node.cpp

TransformNode.o:
	@$(CC) -c TransformNode.cpp

BasicObject.o:
	@$(CC) -c BasicObject.cpp $(FLAGS)

InstanceObject.o:
	@$(CC) -c InstanceObject.cpp $(FLAGS)

Scene.o: Scene.h
	@$(CC) -c Scene.cpp $(FLAGS)

Picture.o:
	@$(CC) -c Picture.cpp $(FLAGS)

Window.o:
	@$(CC) -c Window.cpp $(FLAGS)
clean:
	rm *.o
	rm Main
