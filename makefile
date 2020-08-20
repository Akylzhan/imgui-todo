SRC_FILES = src/main.cpp

LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
LIBS += -lglfw

OBJ_FILES = build/imgui.o build/imgui_demo.o build/imgui_draw.o
OBJ_FILES += build/imgui_impl_glfw.o build/imgui_impl_opengl2.o build/imgui_widgets.o

CXX_FLAGS = -Wall -Wextra -std=c++17 -I libs/imgui -I libs/
CXX = g++

.PHONY : clean format

all: $(OBJ_FILES) 
	$(CXX) -o build/main $(SRC_FILES) $(OBJ_FILES) $(CXX_FLAGS) $(LIBS)
	cp Cousine-Bold.ttf build/Cousine-Bold.ttf
	cp Cousine-Regular.ttf build/Cousine-Regular.ttf

build/%.o: libs/imgui/%.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS)

format:
	clang-format -i -style=file src/main.cpp src/weeks_list.h

clear:
	rm build/*