all: tp.bin

tp.bin: tp.cpp motor.o
	g++ tp.cpp motor.o -o tp.bin -std=c++14

motor.o: Motor.cpp Motor.hpp
	g++ Motor.cpp -o motor.o -c -W -Wall -ansi -pedantic -std=c++14

clean:
	rm -rf *.o *~ tp.bin