echo @off
del *.o

PATH=PATH;c:\mingw\bin;

g++ -c Vertex.cpp
g++ -c AffineTransforms.cpp
g++ -c BasicMatrix.cpp
g++ -c BasicObject.cpp
g++ -c Matrix.cpp
g++ -c Color.cpp
g++ -c Picture.cpp
g++ -c Window.cpp
g++ -c FileIO.cpp
g++ -c DeleteObject.cpp
g++ -c Node.cpp
g++ -c InstanceObject.cpp
g++ -c Scene.cpp
g++ -c Vector.cpp
g++ -c Light.cpp
g++ -c TransformNode.cpp
g++ -c Texture.cpp
g++ -c ReadObject.cpp
g++ -o Main.exe Main.cpp Picture.o Window.o Matrix.o BasicMatrix.o Texture.o Color.o Vertex.o AffineTransforms.o BasicObject.o FileIO.o DeleteObject.o Node.o InstanceObject.o Vector.o Light.o Scene.o TransformNode.o ReadObject.o -lglut32 -lopengl32 -lglew32 -lglu32
Main
