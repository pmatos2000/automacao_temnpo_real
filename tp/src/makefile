all: tp.bin

tp.bin: tp.cpp motor.o controlador.o util.o
	g++ tp.cpp motor.o controlador.o util.o -o tp.bin -std=c++14 -pthread -lboost_system -lboost_date_time -lboost_thread -O3

motor.o: Motor.cpp Motor.hpp util.o
	g++ Motor.cpp -o motor.o -c -W -Wall -ansi -pedantic -std=c++14 -O3

controlador.o: Controlador.cpp Controlador.hpp
	g++ Controlador.cpp -o controlador.o -c -W -Wall -ansi -pedantic -std=c++14 -O3

util.o: Util.cpp Util.hpp
	g++ Util.cpp -o util.o -c -W -Wall -std=c++14 -O3

clean:
	rm -rf *.o *~ tp.bin -O3