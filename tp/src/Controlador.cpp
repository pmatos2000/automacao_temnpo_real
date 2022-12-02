#include "Controlador.hpp"

const double k = 0.001;

void Controlador::set_velocidade_referencia(double velocidade_referencia)
{
    this->velocidade_referencia = velocidade_referencia;
}

double Controlador::atualizar_tensao(double valor_medido)
{
    double nova_tensao = tensao_atual + k * (velocidade_referencia - valor_medido);
    tensao_atual = nova_tensao;
    return nova_tensao;
}

double Controlador::obter_tensao_atual()
{
    return tensao_atual;
}