#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>

/*  Por fim, ajuste o programa para que ele dispare um novo processo 
(via fork(), multiprocess,etc...) chamado synoptic process:
    O synoptic process deve emular um sistema supervisorio para a teleoperacao do
controle do sistema do banco. Esse processo deve trocar informacoes 
com a soft PLC thread, via interface de socket TCP/IP, mostrando na tela do computador os
valores de Ωi,e permitindo que se leia do teclado valores de Ωi ref .
– Todas essas informacoes devem ser registradas em um arquivo denominado 
“historiador.txt”. Aqui, a interface grafica do sinotipo deve ser a mais 
simples possıvel, sem o uso de outras bibliotecas. 
Use o proprio terminal para das saıdas das variaveis.*/

/*
- blocking_tcp_echo_server.cpp
- blocking_tcp_echo_client.cpp
- async_tcp_echo_server.cpp*/


 //divisao dos processos

void handle_connect(boost::system::error_code ec)
{
if (!ec)
{
...
}
}
int main()
{
 boost::asio::io_service io_service;
 boost::asio::ip::tcp::socket socket(io_service);
 socket.async_connect(server_endpoint,
 handle_connect);

 io_service.run();
 return(0);
}