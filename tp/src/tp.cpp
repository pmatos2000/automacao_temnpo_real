#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

#include "Motor.hpp"
#include "Controlador.hpp"
#include "Util.hpp"

#include <unistd.h>

using namespace std;

vector<Motor> motores(30);
vector<Controlador> controladores(30);


void simular_motor(int index_notor)
{
	Motor &motor = motores[index_notor];
	Controlador &controlador = controladores[index_notor];
	double tensao_entrada = 0;

	while (true)
	{
		double velocidade_atual = motor.atualizar_velocidade(tensao_entrada);
		tensao_entrada = controlador.atualizar_tensao(velocidade_atual);
		usleep(200);	
	}
	
}

void log(void)
{
	ofstream arquivo("log.txt");
	
	double tempo_inicial = Util::obter_tempo();
	double tempo_atual = tempo_inicial;

	while (true)
	{
		double delta_tempo = (tempo_atual - tempo_inicial)/1e6;
		for(int i = 0; i  < 30; i++)
		{
			arquivo << i << ' ' << delta_tempo << ' ' << motores[i].get_velocidade_atual() << endl;
		}
		sleep(1);
		tempo_atual = Util::obter_tempo();
	}
	
}

int main() {

	vector<thread> processos(0);

	processos.push_back(thread(log));

	for(int i = 0; i  < 30; i++)
	{
		processos.push_back(thread(simular_motor, i));
	}

	for(int i = 0; i < 12; i++)
	{
		int id = 2*i+1;
		controladores[id].set_velocidade_referencia(100);

	}

	for (auto &processo : processos)
	{
		processo.join();
	}
	


	return 0;
}
