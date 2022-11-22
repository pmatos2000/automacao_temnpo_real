all: tp.bin

tp.bin: tp.cpp motor.o controlador.o
	g++ tp.cpp motor.o  controlador.o -o tp.bin -std=c++14

motor.o: Motor.cpp Motor.hpp
	g++ Motor.cpp -o motor.o -c -W -Wall -ansi -pedantic -std=c++14

controlador.o: Controlador.cpp Controlador.hpp
	g++ Controlador.cpp -o controlador.o -c -W -Wall -ansi -pedantic -std=c++14

clean:
	rm -rf *.o *~ tp.bin