s-libmem: s-libmem.c util.c
	gcc -o s-libmem s-libmem.c util.c -lm