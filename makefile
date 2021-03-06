SRC_FILES = src/main.cpp

UNAME_S := $(shell uname -s)

LIBS =

OBJ_FILES = build/imgui.o build/imgui_demo.o build/imgui_draw.o
OBJ_FILES += build/imgui_impl_glfw.o build/imgui_impl_opengl2.o build/imgui_widgets.o

CXX_FLAGS = -Wall -Wextra -std=c++17
INCLUDE = -I build/ -I libs/imgui -I libs/imgui/examples
INCLUDE += -I libs/imgui/examples/libs/glfw/include/ -I libs/imgui/examples/libs/gl3w/
CXX = g++

ifeq ($(UNAME_S), Linux) #LINUX
	LIBS += -lGL `pkg-config --static --libs glfw3`

	INCLUDE += `pkg-config --cflags glfw3`
endif


ifeq ($(UNAME_S), Darwin) #APPLE
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -lglfw
endif

.PHONY : clean format

all: $(OBJ_FILES) build/date_utilities.a
	@echo $(UNAME_S)
	$(CXX) -o build/main $(SRC_FILES) $(OBJ_FILES) build/date_utilities.a $(CXX_FLAGS) $(INCLUDE) $(LIBS)
	cp Cousine-Bold.ttf build/Cousine-Bold.ttf
	cp Cousine-Regular.ttf build/Cousine-Regular.ttf

build/%.o: libs/imgui/%.cpp
	mkdir -p build
	$(CXX) -c -o $@ $< $(CXX_FLAGS) $(INCLUDE)

build/imgui_impl_glfw.o: libs/imgui/examples/imgui_impl_glfw.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS) $(INCLUDE)

build/imgui_impl_opengl2.o: libs/imgui/examples/imgui_impl_opengl2.cpp
	$(CXX) -c -o $@ $< $(CXX_FLAGS) $(INCLUDE)


build/date_utilities.a: src/date_utilities.go
	go build -buildmode=c-archive -o date_utilities.h -o $@ $^

format:
	clang-format -i -style=file src/main.cpp src/weeks_list.h

clean:
	rm -f build/*