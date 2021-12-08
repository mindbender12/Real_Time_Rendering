CXX := g++ -c
LD := g++
RM := rm -f

INCLUDES := -I/usr/include/freetype2
CXXFLAGS := -O3 -pthread -DGL_GLEXT_PROTOTYPES
LIBS := -lglut -lGL -lGLU -lfreetype
LDFLAGS := -pthread

PRODUCTS := water
OBJECTS := main.o Controller.o CubeMap.o Drawable.o FocusedCamera.o Font.o GLView.o GUIDrawable.o Light.o Mesh.o MeshDrawable.o ParentDrawable.o PositionedCamera.o Quaternion.o Shader.o Shapes.o Skybox.o TestDrawable.o Terrain.o TextDrawable.o Texture.o Utilities.o Vector.o
SOURCES := $(OBJECTS:.o=.cpp)

.PHONY: all clean

all: $(PRODUCTS)

clean:
	$(RM) $(PRODUCTS) $(OBJECTS)

water: $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $<

include depend
