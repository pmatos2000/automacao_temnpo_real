#include "CaixaMensagem.hpp"

void CaixaMensagem::adicionar_mensagem(int id_motor, double velocidade_maxima)
{
    lista_mensagem.push_back({id_motor, velocidade_maxima});
}

bool CaixaMensagem::ler_mensagem_topo(DadosMensagem &dados)
{
    if (lista_mensagem.size() > 0)
    {
        dados = lista_mensagem[0];
        return true;
    }
    return false;
}

void CaixaMensagem::remover_mensagem_topo()
{
    lista_mensagem.erase(lista_mensagem.begin());
}

