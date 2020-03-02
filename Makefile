all: compile run

compile:
	gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives

run:
	./teste