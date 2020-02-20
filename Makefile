all: compile

compile:
	gcc -Wall main.c -o teste -lallegro -lallegro_image -lallegro_primitives
