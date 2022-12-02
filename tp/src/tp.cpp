#include <iostream>
#include <thread>
#include <vector>
#include <fstream>


#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>


#include "Motor.hpp"
#include "Controlador.hpp"
#include "Util.hpp"

#include <unistd.h>


const int QUANTIDADE_MOTORES = 30;
const int QUANTIDADE_MAXIMA_MOTORES_LIGADOS = 12;

/*
*  Valor maximo para não desestabilizar o sistema 10ms não conseguir fazer com 100ms
*  talvez mudando as constantes dos motores consiga os 100ms
*/
const auto TEMPO_SIMULAR_MOTOR = boost::asio::chrono::milliseconds(10); 
const auto TEMPO_ESCREVER_LOG = boost::asio::chrono::seconds(1);
const auto TEMPO_ATUALIZAR_CONTROLE = boost::asio::chrono::milliseconds(200);

const double VELOCIDADE_INICIAL = 500;

using namespace std;

static vector<Motor> motores(30);
static vector<Controlador> controladores(30);


void simular_motor(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador, int index_motor, double tempo_ultima_atualizacao)
{
	//ADICIONAR RESTICAO DE ESPERAR ATUALIZAR OS CONTROLADORES
	//ADICIONAR RESTRICAO DE ESPERAR TERMINAR DE FAZER LOG

	const double tempo_atual = Util::obter_tempo();
	const double delta_tempo =  Util::calcular_delta_tempo(tempo_ultima_atualizacao, tempo_atual);

	const double tensao_entrada = controladores[index_motor].obter_tensao_atual();
	motores[index_motor].atualizar_velocidade(delta_tempo, tensao_entrada);
	
	temporizador->expires_at(temporizador->expiry() + TEMPO_SIMULAR_MOTOR);
	temporizador->async_wait(boost::bind(simular_motor, boost::asio::placeholders::error, temporizador, index_motor, tempo_atual));

	cout << index_motor << " " << delta_tempo  << " " << motores[index_motor].get_velocidade_atual() << endl;
}


void atualizar_tensao_controle(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador)
{
	//COmecar a rotina se não tiver nenhum motor sendo atualizado
	for(int i = 0; i < QUANTIDADE_MOTORES; i++)
	{
		double tensao_atual =  motores[i].get_velocidade_atual();
		controladores[i].atualizar_tensao(tensao_atual);
	}

	temporizador->expires_at(temporizador->expiry() + TEMPO_ATUALIZAR_CONTROLE);
	temporizador->async_wait(boost::bind(atualizar_tensao_controle, boost::asio::placeholders::error, temporizador));
}


//Tem que tornar essa escrita async
//Criar um buff pra salvar melhor os dados
void log_dados(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador, ofstream *arquivo, double tempo_inicial)
{
	//ADICIONAR RESTRICAO DE ESPERAR TODOS MOTORES ESPERAR

	const double tempo_atual = Util::obter_tempo();
	const double delta_tempo =  Util::calcular_delta_tempo(tempo_inicial, tempo_atual);

	for(int i = 0; i < QUANTIDADE_MOTORES; i++)
	{
			(*arquivo) << i << ' ' << delta_tempo << ' ' << motores[i].get_velocidade_atual() << endl;
	}

	temporizador->expires_at(temporizador->expiry() + TEMPO_ESCREVER_LOG);
	temporizador->async_wait(boost::bind(log_dados, boost::asio::placeholders::error, temporizador, arquivo, tempo_inicial));
}


int main(void) {

	boost::asio::io_context contexto;
	vector<boost::asio::steady_timer*> lista_temporizador;

	boost::asio::steady_timer* temporizador = 0;
	double tempo_inicial = Util::obter_tempo();
	

	for(int i = 0; i < QUANTIDADE_MOTORES; i++)
	{
		temporizador = new boost::asio::steady_timer(contexto, TEMPO_SIMULAR_MOTOR);
		temporizador->async_wait(boost::bind(simular_motor, boost::asio::placeholders::error, temporizador, i, tempo_inicial));
		lista_temporizador.push_back(temporizador);
	}
	
	//Motores iniciais ligados 1, 3, 5, ..., 23
	for(int i = 0; i < QUANTIDADE_MAXIMA_MOTORES_LIGADOS; i++)
	{
		controladores[2*i+1].set_velocidade_referencia(VELOCIDADE_INICIAL);
	}
	


	temporizador = new boost::asio::steady_timer(contexto, TEMPO_ATUALIZAR_CONTROLE);
	temporizador->async_wait(boost::bind(atualizar_tensao_controle, boost::asio::placeholders::error, temporizador));



	ofstream arquivo("log.txt");
	temporizador = new boost::asio::steady_timer(contexto, TEMPO_ESCREVER_LOG);
	temporizador->async_wait(boost::bind(log_dados, boost::asio::placeholders::error, temporizador, &arquivo, tempo_inicial));
	lista_temporizador.push_back(temporizador);

	contexto.run();

	return 0;
}
