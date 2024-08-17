# Set it to 1 for da Web
web = 1

cc = gcc
build = build
bin = main
c_src = $(wildcard src/*.c)
h_src = $(wildcard src/*.h)
output = $(build)/$(bin)

test_src = $(wildcard test/*.c)

macros = -DSOKOL_GLCORE
headers = -Isokol -Isokol/util -IHandmadeMath
libs = -lGL -ldl -lm -lX11 -lasound -lXi -lXcursor

ifeq ($(OS), Windows_NT)
	platform := win32
else
	uname := $(shell uname -s)
	ifeq ($(uname), Linux)
		platform := linux
	endif
	ifeq ($(uname), Darwin)
		platform := osx
	endif
endif

shader_bin = sokol-tools-bin/bin/$(platform)/sokol-shdc

shader_src = $(wildcard src/shaders/*.glsl)
shader_out = $(foreach shader, $(subst src/shaders, src/shaders_h, $(shader_src)), $(shader).h)
shader_lang := glsl430

ifeq ($(web), 1)
	cc := emcc
	shader_lang := glsl300es
	macros := -DSOKOL_GLES3
	bin := index
	libs := -sUSE_WEBGL2 --shell-file=res/shell.html
	output := $(build)/$(bin).html
endif


src/shaders_h/%.h: $(shader_src)
	$(shader_bin) -i $< -o $@ -l $(shader_lang)

default:
	make clean
	make shader
	make source

source: $(output)
shader: $(shader_out)

$(output): $(c_src) $(h_src)
	bear -- $(cc) -o $(output) $(c_src) $(headers) $(libs) $(macros)

test:
	$(cc) -o $(build)/test tests/parser_test.c src/parser.c -I./src/

clean:
	rm -f src/shaders_h/*
	rm -f build/*

