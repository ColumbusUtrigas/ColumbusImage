CC = gcc
SRC = ClmbsImg_BMP.c ClmbsImg_JPG.c ClmbsImg_PNG.c ClmbsImg_TGA.c ClmbsImg_TIF.c ClmbsImg_Util.c ColumbusImage.c
LIB = -lpng16 -ljpeg -ltiff

all:
	$(CC) $(SRC) -c $(LIB)
	ar rcs libColumbusImage.a *.o 
	rm -f *.o

