#include <iostream>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int socketDescription;
struct sockaddr_in server;
int port = 80;

int main(int argc, char const *argv[])
{
    struct hostent * host = gethostbyname("localhost");//Obtene el host de Google
    char  server_reply[2000];
    if ( (host == NULL) || (host->h_addr == NULL) ) {
        cout << "Error al obtener host." << endl;
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
    memcpy(&server.sin_addr, host->h_addr, host->h_length);

    socketDescription = socket(AF_INET, SOCK_STREAM, 0);

    if (socketDescription < 0) {
        cout << "Error al crear el socket." << endl;
        exit(1);
    }
    cout << "Conectando..."  << endl;
    if ( connect(socketDescription, (struct sockaddr *)&server, sizeof(server)) < 0 ) {
        close(socketDescription);
        cout << "No se pudo establecer la conexion" << endl;
        exit(1);
    }//Conectandose al servidor

    stringstream ss;
    ss << "GET / HTTP/1.0\r\n\r\n";
    string request = ss.str();
    
    if (send(socketDescription, request.c_str(), request.length(), 0) != (int)request.length()) {
        cout << "Error al enviar mensaje." << endl;
        exit(1);
    }//enviar data al servidor
    cout << "Mensaje enviado..." << endl;
    if (recv(socketDescription, server_reply, 2000,0) < 0){
    	cout << "Error al recibir respuesta"<< endl;
    	exit(1);
    }
    cout << "Respuesta recibida" << endl;
    cout << server_reply << endl;
    close(socketDescription);//Cerrando el socket
    cout << "Desconectando..." << endl;
    return 0;
}