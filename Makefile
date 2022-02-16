# Dr.LUT - Lookup Table Generator
#
# Copyright (c) 2021 by ppelikan
# github.com/ppelikan


IMGUI_DIR = ../imgui
IMPLOT_DIR = ../implot
FD_DIR = ext/ImGuiFileDialog
SRC_DIR = src

SOURCES = $(SRC_DIR)/main.cpp\
		  $(SRC_DIR)/gui_app_main.cpp\
		  $(SRC_DIR)/lut_builder.cpp\
		  $(SRC_DIR)/wave_generator.cpp\
		  $(SRC_DIR)/multitype_array.cpp

SOURCES += $(IMGUI_DIR)/imgui.cpp\
		   $(IMGUI_DIR)/imgui_draw.cpp\
		   $(IMGUI_DIR)/imgui_tables.cpp\
		   $(IMGUI_DIR)/imgui_widgets.cpp\
		   $(IMGUI_DIR)/misc/cpp/imgui_stdlib.cpp\
		   $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp\
		   $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp\
		   $(IMPLOT_DIR)/implot.cpp\
		   $(IMPLOT_DIR)/implot_items.cpp

INCLUDES = -I$(IMGUI_DIR)\
		   -I$(IMGUI_DIR)/misc/cpp\
		   -I$(IMGUI_DIR)/backends\
		   -I$(IMPLOT_DIR)

PORT_SOURCES = $(SOURCES) $(FD_DIR)/ImGuiFileDialog.cpp
PORT_INCLUDES = $(INCLUDES) -I$(FD_DIR)

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------
WASM_BUILD_DIR = build/wasm
WASM_CC = emcc
WASM_CXX = em++
WEB_DIR = $(WASM_BUILD_DIR)/web
WASM_EXE = $(WEB_DIR)/index.html
WASM_EMS += -s USE_SDL=2 -s WASM=1
WASM_EMS += -s ALLOW_MEMORY_GROWTH=1
WASM_EMS += -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0
WASM_EMS += -s ASSERTIONS=1
WASM_EMS += -s NO_FILESYSTEM=1 -DIMGUI_DISABLE_FILE_FUNCTIONS

WASM_CPPFLAGS = $(INCLUDES)
WASM_CPPFLAGS += -Wall -Wformat -Os
WASM_CPPFLAGS += $(WASM_EMS) -DWASM_BUILD
WASM_LIBS += $(WASM_EMS)
WASM_LDFLAGS += --shell-file shell.html


##---------------------------------------------------------------------
## PORTABLE OPTIONS
##---------------------------------------------------------------------
PORT_BUILD_DIR = build/portable
PORT_CC = gcc
PORT_CXX = g++
PORT_EXE = $(PORT_BUILD_DIR)/app_executable
LINUX_GL_LIBS = -lGLESv2
PORT_CXXFLAGS = $(PORT_INCLUDES)
# Debugging:
PORT_CXXFLAGS += -Wall -Wformat -g -Og 
# Release:
#PORT_CXXFLAGS += -O2
PORT_LIBS =
PORT_CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	PORT_LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	PORT_CXXFLAGS += `sdl2-config --cflags`
	PORT_CFLAGS = $(PORT_CXXFLAGS)
endif

# ifeq ($(UNAME_S), Darwin) #APPLE
# 	ECHO_MESSAGE = "Mac OS X"
# 	PORT_LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
# 	PORT_LIBS += -L/usr/local/lib -L/opt/local/lib

# 	PORT_CXXFLAGS += `sdl2-config --cflags`
# 	PORT_CXXFLAGS += -I/usr/local/include -I/opt/local/include
# 	PORT_CFLAGS = $(PORT_CXXFLAGS)
# endif

# ifeq ($(OS), Windows_NT)
#     ECHO_MESSAGE = "MinGW"
#     PORT_LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

#     PORT_CXXFLAGS += `pkg-config --cflags sdl2`
#     PORT_CFLAGS = $(PORT_CXXFLAGS)
# endif


WASM_OBJS = $(addprefix $(WASM_BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
PORT_OBJS = $(addprefix $(PORT_BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(PORT_SOURCES)))))

ICON = favicon.ico

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(WEB_DIR):
	mkdir $@

$(WASM_BUILD_DIR):
	mkdir -p $@

$(WASM_EXE): $(WASM_BUILD_DIR) $(WASM_OBJS) $(WEB_DIR) $(WEB_DIR)/favicon.ico
	$(WASM_CXX) -o $@ $(WASM_OBJS) $(WASM_LIBS) $(WASM_LDFLAGS)
	@echo Building for WEB completed

$(WASM_BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(WASM_CXX) $(WASM_CPPFLAGS) $(WASM_CXXFLAGS) -c -o $@ $<

$(WASM_BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(WASM_CXX) $(WASM_CPPFLAGS) $(WASM_CXXFLAGS) -c -o $@ $<

$(WASM_BUILD_DIR)/%.o:$(IMGUI_DIR)/misc/cpp/%.cpp
	$(WASM_CXX) $(WASM_CPPFLAGS) $(WASM_CXXFLAGS) -c -o $@ $<

$(WASM_BUILD_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(WASM_CXX) $(WASM_CPPFLAGS) $(WASM_CXXFLAGS) -c -o $@ $<

$(WASM_BUILD_DIR)/%.o:../libs/gl3w/GL/%.c
	$(WASM_CC) $(WASM_CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(WASM_BUILD_DIR)/%.o:$(IMPLOT_DIR)/%.cpp
	$(WASM_CXX) $(WASM_CPPFLAGS) $(WASM_CXXFLAGS) -c -o $@ $<

$(WEB_DIR)/favicon.ico: $(ICON)
	cp $(ICON) $(WEB_DIR)/favicon.ico

$(PORT_BUILD_DIR):
	mkdir -p $@

$(PORT_EXE): $(PORT_BUILD_DIR) $(PORT_OBJS)
	$(PORT_CXX) -o $@ $(PORT_OBJS) $(PORT_CXXFLAGS) $(PORT_LIBS)
	@echo Building for $(ECHO_MESSAGE) completed

$(PORT_BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

$(PORT_BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

$(PORT_BUILD_DIR)/%.o:$(IMGUI_DIR)/misc/cpp/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

$(PORT_BUILD_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

$(PORT_BUILD_DIR)/%.o:$(IMPLOT_DIR)/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

$(PORT_BUILD_DIR)/%.o:$(FD_DIR)/%.cpp
	$(PORT_CXX) $(PORT_CXXFLAGS) -c -o $@ $<

portable: $(PORT_EXE)

web: $(WASM_EXE)

all: $(PORT_EXE) $(WASM_EXE)
	@echo Build ALL completed

serve: $(WASM_EXE)
	python3 -m http.server -d $(WEB_DIR)

clean:
	rm -rf $(WASM_OBJS) $(WEB_DIR) $(WASM_BUILD_DIR) $(PORT_BUILD_DIR)
