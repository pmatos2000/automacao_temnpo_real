#include "rede.hpp"
#include "controlador.hpp"
#include "motor.hpp"
#include <iostream>



void softPLC(){

    Controlador cont;
    Motor motor;
    double v_referencia;

    //Mostrar a velocidade atual
    std::cout<<motor.get_velocidade_atual()<<std::endl;

    //Ler a velocidade de referencia
    std::cin>>v_referencia;

}











void synoptic_process(){
    //Criar um socket TCP/IP 
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

    //Conectar o socket à porta onde o servidor se encontra
    server_address = ('localhost', 10000);
    print('connecting to %s port %s' % server_address);
    sock.connect(server_address);
}
    try:

        //enviar dados
        message = 'This is the message.  It will be repeated.'
        print('sending "%s"' % message)
        sock.sendall(message)

        //Espera a resposta
        amount_received = 0
        amount_expected = len(message)

        while amount_received < amount_expected:
            data = sock.recv(16)
            amount_received += len(data)
            print('received "%s"' % data)

    finally:
        print('closing socket')
        sock.close()
        '''

    
if __name__ == "__main__": 
  
    # print ID of current process 
    print("ID of process running main program: {}".format(os.getpid()))
  
    # print name of main thread 
    print("Main thread name: {}".format(threading.current_thread().name))

    # Create a TCP/IP socket
    '''
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the port
    server_address = ('localhost', 10000)
    print('iniciando %s na porta %s' % server_address)
    sock.bind(server_address)

    sock.listen(1)

    while True:
        # Wait for a connection
        print('waiting for a connection')
        connection, client_address = sock.accept()
        connection.close()

        t0 = threading.Thread(target=synoptic_process(), name='t0')

        t0.start()
        t0.join()