#include <chrono>
#include <thread>
#include <iostream>

#include "Motor.hpp"

const double Ra = 7.2; // Resistência da amadura
const double La = 3.4e-3; // Indutancia amadura
const double Km = 13.15e-3; // Constante de toque
const double tal_L = 0; // Pertubação
const double Jm = 4.5e-7; // Momento de inércia
const double B = 3.95e-5; // Atrito viscoso
const double Kb = 0.0132; //Constante elétrica

double Motor::get_velocidade_atual()
{
	return velocidade_atual;
}

double derivada_toque(double tal_m, double wm, double V)
{

	return (Km * V - Km * Kb * wm - Ra*tal_m);
}

double derivada_velocidade(double tal_m, double wm)
{
	return (tal_m - tal_L - B * wm)/Jm;
}



double Motor::atualizar_velocidade(double delta_tempo, double tensao_entrada)
{

	
	// std::cout << "tempo: " << delta_t << std::endl;

	double k1_toque = derivada_toque(toque_atual, velocidade_atual, tensao_entrada);
	double k1_velocidade = derivada_velocidade(toque_atual, velocidade_atual);

	toque_atual = toque_atual + delta_tempo * k1_toque;
	velocidade_atual = velocidade_atual + delta_tempo * k1_velocidade;

	// std::cout << "velocidade: " << velocidade_atual << std::endl;
	// std::cout << "toque: " << toque_atual << std::endl;

	return velocidade_atual;

	/*

	double k1_toque = derivada_toque(toque_atual, velocidade_atual, tensao_entrada);
	double k1_velocidade = derivada_velocidade(toque_atual, velocidade_atual);

	double toque_aux =  toque_atual + 0.5 * delta_t * k1_toque;
	double velocidade_aux = velocidade_atual + 0.5 * delta_t * k1_velocidade;
	double k2_toque = derivada_toque(toque_aux, toque_aux, tensao_entrada);
	double k2_velocidade = derivada_velocidade(toque_aux, velocidade_aux);

	toque_aux =  toque_atual + 0.5 * delta_t * k2_toque;
	velocidade_aux = velocidade_atual + 0.5 * delta_t * k2_velocidade;
	double k3_toque = derivada_toque(toque_aux, toque_aux, tensao_entrada);
	double k3_velocidade = derivada_velocidade(toque_aux, velocidade_aux);

	toque_aux =  toque_atual + delta_t * k3_toque;
	velocidade_aux = velocidade_atual + delta_t * k3_velocidade;
	double k4_toque = derivada_toque(toque_aux, toque_aux, tensao_entrada);
	double k4_velocidade = derivada_velocidade(toque_aux, velocidade_aux);

	toque_atual += delta_t * (k1_toque + 2 * k2_toque + 2 * k3_toque + k4_toque)/6;
	velocidade_atual += delta_t * (k1_velocidade + 2 * k2_velocidade + 2 * k3_velocidade + k4_velocidade)/6;
	tempo_ultima_atualizacao = tempo;
	*/

}