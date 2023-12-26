CFLAGS 		:= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS 	:= $(CFLAGS)
LDFLAGS 	:= -lGL -lglfw -lGLEW -lpng
OBJ 		:= main.o shader.o image.o
GLSL_H		:= vert.glsl.h frag.glsl.h
BIN 		:= gldvd


$(BIN): $(OBJ)
	$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)

main.o: main.cpp $(GLSL_H)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cpp 
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.glsl.h: %.glsl
	tts $< > $@

clean:
	rm -f $(OBJ) $(BIN) $(GLSL_H)

.PHONY: $(BIN) clean
