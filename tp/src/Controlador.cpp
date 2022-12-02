#include "Controlador.hpp"

const double k = 0.02;

void Controlador::set_velocidade_referencia(double velocidade_referencia)
{
    this->velocidade_referencia = velocidade_referencia;
}

void Controlador::atualizar_tensao(double valor_medido)
{
    tensao_atual = tensao_atual + k * (velocidade_referencia - valor_medido);
}

double Controlador::obter_tensao_atual()
{
    return tensao_atual;
}