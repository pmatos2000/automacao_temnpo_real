#include "Controlador.hpp"
#include "Util.hpp"

#include <iostream>
using namespace std;

const double k = 0.02;

const double TEMPO_MAXIMO_CONTROLE  = 60; // 60s

void Controlador::set_velocidade_referencia(double velocidade_referencia)
{
    this->velocidade_referencia = velocidade_referencia;
    inicio_controle = Util::obter_tempo();
}

void Controlador::atualizar_tensao(double valor_medido)
{
    if(!verificar_controle_ativo()) return;
    tensao_atual = tensao_atual + k * (velocidade_referencia - valor_medido);
}

bool Controlador::verificar_controle_ativo()
{
    double delta_tempo = Util::calcular_delta_tempo(inicio_controle, Util::obter_tempo());
    if( velocidade_referencia == 0 || delta_tempo >= TEMPO_MAXIMO_CONTROLE) return false;
    return true;
}

double Controlador::obter_tensao_atual()
{
    if(!verificar_controle_ativo()) return 0;
    return tensao_atual;
}