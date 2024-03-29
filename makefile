CXX = g++
# CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -std=c++17 -pthread -o bin/mandelbrot `wx-config --libs` `wx-config --cxxflags`
CXXFLAGS = -std=c++17 -pthread #`wx-config --libs` `wx-config --cxxflags`
WXFLAGS = `wx-config --libs` `wx-config --cxxflags`

# https://stackoverflow.com/questions/714100/os-detecting-makefile
 UNAME_S := $(shell uname -s)
 ifeq ($(UNAME_S),Linux)
# 	OPENCL_FLAGS = -lOpenCL
 	WXTHING = wx-config # used to be wx-config-gtk3 ?? 
 endif
 ifeq ($(UNAME_S),Darwin)
# 	OPENCL_FLAGS = -framework OpenCl
 	WXTHING = wx-config
 endif

OBJECTS = bin/GUI.o bin/Game.o bin/NeuralNet.o

# @ supresses output from command
all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) `$(WXTHING) --libs` -Ofast -o bin/flappy

bin/GUI.o: src/GUI.cpp
	$(CXX) $^ $(CXXFLAGS) `$(WXTHING) --cxxflags` -c -o $@ -g

bin/Game.o: src/Game.cpp
	$(CXX) $^ $(CXXFLAGS) `$(WXTHING) --cxxflags` -c -o $@ -g

bin/NeuralNet.o: src/NeuralNet.cpp
	$(CXX) $^ $(CXXFLAGS) -c -o $@ -g

debug: $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) `$(WXTHING) --libs` -g -o bin/flappy

clean:
	$(RM) bin/flappy $(OBJECTS)
