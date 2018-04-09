all: z1 z1.1 z1.2 z2 z3 main

z1: 
	g++ z1.cpp
	./a.out

z1.1: 
	g++ z1.1.cpp
	./a.out

z1.2: 
	g++ z1.2.cpp
	./a.out

z2:
	g++ z2.cpp
	./a.out

z3:
	g++ z3.cpp
	./a.out

main:
	g++ main.cpp
	./a.out