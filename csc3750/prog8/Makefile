CC = g++
INCLUDE = -I/usr/include
FLAGS = -O2
FILENAME = Main
LDLIBS =   -lglut -lGL -lGLU -lGLEW
LDFLAGS =  -L/usr/lib -L/lib

all: Main.cpp Picture.o Window.o Matrix.o BasicMatrix.o Color.o Vertex.o AffineTransforms.o DeleteObject.o BasicObject.o InstanceObject.o Scene.o Node.o Vector.o TransformNode.o Light.o Texture.o ReadObject.o FileIO.o
	@$(CC) Main.cpp Picture.o Window.o Matrix.o BasicMatrix.o Color.o Vertex.o Vector.o AffineTransforms.o DeleteObject.o BasicObject.o Node.o InstanceObject.o Scene.o Light.o TransformNode.o Texture.o ReadObject.o FileIO.o $(LDLIBS) -o $(FILENAME) $(FLAGS)

ReadObject.o:
	@$(CC) -c ReadObject.cpp $(FLAGS)

BasicMatrix.o:
	@$(CC) -c BasicMatrix.cpp $(FLAGS)

Matrix.o:
	@$(CC) -c Matrix.cpp $(FLAGS)

FileIO.o:
	@$(CC) -c FileIO.cpp $(FLAGS)
	
Color.o:
	@$(CC) -c Color.cpp $(FLAGS)

Vertex.o:
	@$(CC) -c Vertex.cpp $(FLAGS)

Vector.o:
	@$(CC) -c Vector.cpp $(FLAGS)

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

Light.o:
	  @$(CC) -c Light.cpp $(FLAGS)

Scene.o: Scene.h
	@$(CC) -c Scene.cpp $(FLAGS)

Picture.o:
	@$(CC) -c Picture.cpp $(FLAGS)

Texture.o:
	@$(CC) -c Texture.cpp $(FLAGS)

Window.o:
	@$(CC) -c Window.cpp $(FLAGS)
clean:
	rm *.o
	rm Main
