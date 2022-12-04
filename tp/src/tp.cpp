#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "Motor.hpp"
#include "Controlador.hpp"
#include "Util.hpp"
#include "CaixaMensagem.hpp"

const int QUANTIDADE_MOTORES = 30;
const int QUANTIDADE_MAXIMA_MOTORES_LIGADOS = 12;

/*
*  Valor maximo para não desestabilizar o sistema 10ms não conseguir fazer com 100ms
*  talvez mudando as constantes dos motores consiga os 100ms
*/
const auto TEMPO_SIMULAR_MOTOR = boost::asio::chrono::milliseconds(10); 
const auto TEMPO_ESCREVER_LOG = boost::asio::chrono::seconds(1);
const auto TEMPO_ATUALIZAR_CONTROLE = boost::asio::chrono::milliseconds(200);

const double VELOCIDADE_INICIAL = 300;

using namespace std;

static vector<Motor> motores(30);
static vector<Controlador> controladores(30);
static CaixaMensagem caixa_mensagem;

mutex mtx_dados_motores;
mutex mtx_dados_controladores;
mutex mtx_caixa_mensagem;

boost::asio::io_context contexto;

void simular_motor(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador, int index_motor, double tempo_ultima_atualizacao)
{	double tempo_atual = 0;
	mtx_dados_motores.lock();
	{
		mtx_dados_controladores.lock();
		{
			tempo_atual = Util::obter_tempo();
			double delta_tempo =  Util::calcular_delta_tempo(tempo_ultima_atualizacao, tempo_atual);
		
			const double tensao_entrada = controladores[index_motor].obter_tensao_atual();
			motores[index_motor].atualizar_velocidade(delta_tempo, tensao_entrada);
		}
		mtx_dados_controladores.unlock();
	}
	mtx_dados_motores.unlock();

	temporizador->expires_at(temporizador->expiry() + TEMPO_SIMULAR_MOTOR);
	temporizador->async_wait(boost::bind(simular_motor, boost::asio::placeholders::error, temporizador, index_motor, tempo_atual));

	// cout << index_motor << " " << delta_tempo  << " " << motores[index_motor].get_velocidade_atual() << endl;
}

inline int obter_quantidade_controles_livres()
{
	int quantidade_controles_usados = 0;
	for(int i = 0; i < QUANTIDADE_MOTORES; i++)
	{
		if(controladores[i].verificar_controle_ativo()) quantidade_controles_usados++;
	}
	return QUANTIDADE_MAXIMA_MOTORES_LIGADOS - quantidade_controles_usados;
}

void atualizar_tensao_controle(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador)
{

	mtx_dados_motores.lock();
	{
		mtx_dados_controladores.lock();
		{
			int quantidade_controles_livres = obter_quantidade_controles_livres();

			mtx_caixa_mensagem.lock();
			{
				while(quantidade_controles_livres > 0)
				{
					DadosMensagem dados_mensagem;
					bool leitura_sucesso = caixa_mensagem.ler_mensagem_topo(dados_mensagem);
					
					if(leitura_sucesso == false) break;
					// cout << "Executadno o comando: "<<  dados_mensagem.id_motor << " " << dados_mensagem.velocidade_maxima << endl;

					if( (dados_mensagem.id_motor == 0 && controladores[dados_mensagem.id_motor+1].verificar_controle_ativo())
					|| (dados_mensagem.id_motor == QUANTIDADE_MOTORES - 1 && controladores[dados_mensagem.id_motor-1].verificar_controle_ativo())
					|| (controladores[dados_mensagem.id_motor-1].verificar_controle_ativo() || controladores[dados_mensagem.id_motor+1].verificar_controle_ativo()))
					{
						// cout << "Falha ao executar o comando" << endl;
						break;
					}

					controladores[dados_mensagem.id_motor].set_velocidade_referencia(dados_mensagem.velocidade_maxima/2);
					caixa_mensagem.remover_mensagem_topo();
					quantidade_controles_livres--;
				}
			}
			mtx_caixa_mensagem.unlock();

			for(int i = 0; i < QUANTIDADE_MOTORES; i++)
			{
				double tensao_atual =  motores[i].get_velocidade_atual();
				controladores[i].atualizar_tensao(tensao_atual);
			}
		}
		mtx_dados_controladores.unlock();
	}
	mtx_dados_motores.unlock();

	temporizador->expires_at(temporizador->expiry() + TEMPO_ATUALIZAR_CONTROLE);
	temporizador->async_wait(boost::bind(atualizar_tensao_controle, boost::asio::placeholders::error, temporizador));
}


void log_dados(boost::system::error_code /*e*/, boost::asio::steady_timer* temporizador, ofstream *arquivo, double tempo_inicial)
{
	stringstream ss;
	mtx_dados_motores.lock();
	{
		const double tempo_atual = Util::obter_tempo();
		const double delta_tempo =  Util::calcular_delta_tempo(tempo_inicial, tempo_atual);
		for(int i = 0; i < QUANTIDADE_MOTORES; i++)
		{
				ss << i << ' ' << delta_tempo << ' ' << motores[i].get_velocidade_atual() << endl;
		}
	}
	mtx_dados_motores.unlock();
	(*arquivo) << ss.rdbuf();

	// cout << ss.str();
	temporizador->expires_at(temporizador->expiry() + TEMPO_ESCREVER_LOG);
	temporizador->async_wait(boost::bind(log_dados, boost::asio::placeholders::error, temporizador, arquivo, tempo_inicial));
}

void interface()
{
	int id_motor;
	double velocidade_maxima;
	while (true)
	{
		cout << "Entre com o ID do Motor (0..29) e com a Velocidade" << endl;
		cout << "Informando o ID = -1 ou ID > 29 o programa finaliza" << endl;

		cin >> id_motor;
		if (id_motor < 0 || id_motor > 29)
		{
			contexto.stop();
			return;
		}

		cin >> velocidade_maxima;
		velocidade_maxima = velocidade_maxima < 0 ? 0 : velocidade_maxima;

		mtx_caixa_mensagem.lock();
		{
			caixa_mensagem.adicionar_mensagem(id_motor, velocidade_maxima);
		}
		mtx_caixa_mensagem.unlock();

		cout << "Comando registrado com sucesso!" << endl;
	}
}


int main(void)
{
	vector<boost::asio::steady_timer*> lista_temporizador;
	boost::asio::steady_timer* temporizador = 0;
	double tempo_inicial = Util::obter_tempo();
	
	for(int i = 0; i < QUANTIDADE_MOTORES; i++)
	{
		temporizador = new boost::asio::steady_timer(contexto, TEMPO_SIMULAR_MOTOR);
		temporizador->async_wait(boost::bind(simular_motor, boost::asio::placeholders::error, temporizador, i, tempo_inicial));
		lista_temporizador.push_back(temporizador);
	}
	
	/*
	//Motores iniciais ligados 1, 3, 5, ..., 23
	for(int i = 0; i < QUANTIDADE_MAXIMA_MOTORES_LIGADOS; i++)
	{
		controladores[2*i+1].set_velocidade_referencia(VELOCIDADE_INICIAL);
	}
	*/
	
	temporizador = new boost::asio::steady_timer(contexto, TEMPO_ATUALIZAR_CONTROLE);
	temporizador->async_wait(boost::bind(atualizar_tensao_controle, boost::asio::placeholders::error, temporizador));

	ofstream arquivo("log.txt");
	temporizador = new boost::asio::steady_timer(contexto, TEMPO_ESCREVER_LOG);
	temporizador->async_wait(boost::bind(log_dados, boost::asio::placeholders::error, temporizador, &arquivo, tempo_inicial));
	lista_temporizador.push_back(temporizador);

	thread interface_thead(interface);	
	
	contexto.run();
	interface_thead.join();


	arquivo.close();
	for(auto &t: lista_temporizador)
	{
		delete t;
	}
	lista_temporizador.clear();

	return 0;
}
