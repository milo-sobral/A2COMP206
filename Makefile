#Makefile for Assignment 2 of Comp 206, McGill Winter 2018
#Author : Milo Sobral

all : bmp_info.o bmp_mask.o bmp_collage.o A2_bmp_helpers.o 
	gcc bmp_info.o A2_bmp_helpers.o -o bmp_info
	gcc bmp_mask.o A2_bmp_helpers.o -o bmp_mask
	gcc bmp_collage.o A2_bmp_helpers.o -o bmp_collage

bmp_info.o : 
	gcc -c src/bmp_info.c

bmp_mask.o : 
	gcc -c src/bmp_mask.c

bmp_collage.o : 
	gcc -c src/bmp_collage.c

A2_bmp_helpers.o : 
	gcc -c src/A2_bmp_helpers.c

clean : 
	rm -rf ./*.o bmp_*