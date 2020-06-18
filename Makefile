all: compile run rm

compile:
	gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives -lallegro_ttf -lallegro_font

run:
	./teste