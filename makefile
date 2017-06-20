CFLAGS = -shared -fpic
OBJ = spam.c

spam.so : spam.c
	gcc $(CFLAGS) $(OBJ) -o spam.so
