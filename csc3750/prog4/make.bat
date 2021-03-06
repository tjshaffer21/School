echo @off
del *.o

PATH=PATH;c:\mingw\bin;

g++ -c Vertex.cpp
g++ -c AffineTransforms.cpp
g++ -c BasicMatrix.cpp
g++ -c BasicObject.cpp
g++ -c Face.cpp
g++ -c Matrix.cpp
g++ -c Color.cpp
g++ -c Pixel.cpp
g++ -c Picture.cpp
g++ -c Window.cpp
g++ -c FileIO.cpp
g++ -c DeleteObject.cpp
g++ -c Node.cpp
g++ -c TransformNode.cpp
g++ -c InstanceObject.cpp
g++ -c Scene.cpp
g++ -c Vector.cpp
g++ -o Main.exe Main.cpp Picture.o Window.o Pixel.o Matrix.o BasicMatrix.o Color.o Vertex.o AffineTransforms.o BasicObject.o Face.o FileIO.o DeleteObject.o Node.o TransformNode.o InstanceObject.o Vector.o Scene.o -lglut32 -lopengl32
Main
