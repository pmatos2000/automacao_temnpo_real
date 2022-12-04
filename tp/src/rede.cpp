# inlcude"rede.hpp"

class softPLC_thread(){

    print('Enter initial conditions:')
    x0 = float(input('x0 = '))
    y0 = float(input('y0 = '))

    print('Enter calculation point: ')
    xn = float(input('xn = '))

    print('Enter number of steps:')
    step = int(input('Number of steps = '))
};    

def logger_thread():
    try:
        log = open("historiador.txt", "x")
    except:
        print("Esse arquivo já existe")
    while (True):
        timestamp = time.time()
        carimbo = datetime.datetime.fromtimestamp(timestamp).strfttime("%H:%M:%S")
        with open("historiador.txt", "a") as arqlog:
            arqlog.write(
                "Os valores dos niveis dos tanques às" + carimbo + ": \n x0: " + str(x0) + "\n y0: " + str(y0) +
                "\n xn: " + str(xn) + "\n\n steps: " + str(step))
        time.sleep(1)


void synoptic_process(){
    //Criar um socket TCP/IP 
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

    //Conectar o socket à porta onde o servidor se encontra
    server_address = ('localhost', 10000);
    print('connecting to %s port %s' % server_address);
    sock.connect(server_address);

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

    //Criando as threads

    t1 = threading.Thread(target=softPLC_thread(), name='t1');
    t2 = threading.Thread(target=process_thread(), name='t2');
    t3 = threading.Thread(target=logger_thread(), name='t3');

    t1.start();
    t1.join();

    t2.start();
    t2.join();

    t3.start();
    t3.join();
};