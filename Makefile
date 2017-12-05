CC = gcc
SRC = ClmbsImg_BMP.c ClmbsImg_PNG.c ClmbsImg_TGA.c ClmbsImg_Util.c ColumbusImage.c

all:
	$(CC) $(SRC) -c
	ar rcs libColumbusImage.a *.o 


