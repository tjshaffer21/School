echo @off
del spcoord.txt
del *.o

PATH=PATH;C:\mingw\bin

g++ -c FileIO.cpp
g++ -c BasicMatrix.cpp
g++ -c Matrix.cpp
g++ -c VertexMin.cpp
g++ -c FaceMin.cpp

g++ SphericalTextureCoord.cpp FileIO.o BasicMatrix.o Matrix.o VertexMin.o FaceMin.o -o spcoord.exe