#include <iostream>
#include "Motor.hpp"
#include "Controlador.hpp"

#include <unistd.h>

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	Motor m1 = Motor();
	Controlador c1 = Controlador();
	c1.set_velocidade_referencia(2022);
	double tensao_entrada = 0;
	for(int i = 0; i  < 10000; i++)
	{
 		double velocidade_atual = m1.atualizar_velocidade(tensao_entrada);
		cout << "velocidade: " << velocidade_atual << endl;
		tensao_entrada = c1.atualizar_tensao(velocidade_atual);
		cout << "tensão de entrada: " << tensao_entrada << endl;
		usleep(200);

	}
	return 0;
}
