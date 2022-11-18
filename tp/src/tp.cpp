#include <iostream>
#include "Motor.hpp"

#include <unistd.h>

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	Motor m1 = Motor();
	for(int i = 0; i  < 10000000; i++)
	{
		cout << m1.get_velocidade_atual() << endl;
		sleep(1);
		m1.atualizar(1);

	}
	return 0;
}
