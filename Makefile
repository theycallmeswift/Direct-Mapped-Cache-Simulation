# Makefile
#
# Author: Mike Swift <theycallmeswift@gmail.com>
#
# Created on: April 28th, 2011
# Modified on: April 28th, 2011
#
# Complile using "make" and clean using "make clean"

CC = gcc
CCFLAGS  = -ansi -pedantic -Wall -g

all: sim

sim: src/sim.c src/sim.h
	$(CC) $(CCFLAGS) -o sim src/sim.c
	mv sim bin/sim
	rm -rf *.o
	
clean:
	rm -rf bin/*

