CC = g++

all: FileIO.o BasicMatrix.o VertexMin.o FaceMin.o Matrix.o
	@$(CC) SphericalTextureCoord.cpp FileIO.o BasicMatrix.o Matrix.o VertexMin.o FaceMin.o -o spcoord

FileIO.o:
	@$(CC) -c FileIO.cpp

BasicMatrix.o:
	@$(CC) -c BasicMatrix.cpp

Matrix.o: BasicMatrix.o
	@$(CC) -c Matrix.cpp

VertexMin.o:
	@$(CC) -c VertexMin.cpp

FaceMin.o: VertexMin.o
	@$(CC) -c FaceMin.cpp

clean:
	rm *.o
	rm spcoord