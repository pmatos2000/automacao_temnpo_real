#include <iostream>
#include "Motor.hpp"

#include <unistd.h>

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	Motor m1 = Motor();
	for(int i = 0; i  < 1000; i++)
	{
 		double v = m1.atualizar_velocidade(12);
		cout << "velocidade: " << v << endl;
		usleep(500);

	}
	return 0;
}
