all: main.c
	gcc -o qtclass-gen main.c

clean:
	rm qtclass-gen
