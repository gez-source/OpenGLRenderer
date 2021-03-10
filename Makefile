IDIR=./includes
ODIR=./obj
SDIR=./src

CXX=g++
CXXFLAGS=-std=c++14 -g -Wall -pedantic -static
CXX_DEFINES = -DGLEW_STATIC -DTW_NO_DIRECT3D -DTW_NO_LIB_PRAGMA -DTW_STATIC -D_CRT_SECURE_NO_WARNINGS
CXX_INCLUDES = -I"./../../external/glfw/include" -I"./../../external/glew/include" -I$(IDIR)
CXX_LIBRARIES=-L"./../../external/glew/lib" -L"./../../external/glfw/build/src"

LIBS_OPENGL=-lm -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXinerama -lXcursor -lrt -lXrandr -pthread -lXi -ldl -lglfw3 -lGLEW
LIBS_SFML=-lm -lsfml-graphics -lsfml-window -lsfml-system
LIBS=$(LIBS_OPENGL) $(LIBS_SFML)

_OBJ = IOHelpers.o Renderer.o 

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

bin/static/libopenglrenderer.a: $(OBJ)
	$(shell mkdir -p bin/static)\
	ar rcs bin/static/libopenglrenderer.a $(OBJ)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(shell mkdir -p obj)\
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBS) $(CXX_INCLUDES) $(CXX_LIBRARIES) $(CXX_DEFINES) 	

.PHONY: clean

clean:
	rm -f bin/static/libopenglrenderer.a \
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ \
	rm -rf obj\
	rm -rf bin

