CFLAGS 		:= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS 	:= $(CFLAGS)
LDFLAGS 	:= -lGL -lglfw -lGLEW -lpng
OBJ 		:= main.o shader.o image.o app.o draw.o
GLSL_H		:= vert.glsl.h frag.glsl.h
BIN 		:= gldvd


$(BIN): $(OBJ)
	@echo "  LD	" $(BIN)
	@$(CXX) -o $(BIN) $(OBJ) $(LDFLAGS)

draw.o: draw.cpp $(GLSL_H)
	@echo "  CXX	" $@
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cpp 
	@echo "  CXX	" $@
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.c
	@echo "  CC	" $@
	@$(CC) -c -o $@ $< $(CFLAGS)

%.glsl.h: %.glsl
	@echo "  TTS	" $@
	@tts $< > $@

clean:
	@echo "  CLEAN	" $(OBJ) $(BIN) $(GLSL_H)
	@rm -f $(OBJ) $(BIN) $(GLSL_H)

.PHONY: $(BIN) clean
