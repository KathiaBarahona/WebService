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
#include <pthread.h>

using namespace std;

int socketDescription, newSocket, size, *newSocketPointer;
struct sockaddr_in server,client;
int port = 80;

void *connectListenner(void *param);
int main(int argc, char const *argv[])
{   
    string message;
    socketDescription = socket(AF_INET, SOCK_STREAM,0);
    if(socketDescription == -1){
        cout << "Error al crear socket" << endl;
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr =INADDR_ANY; //Conectarse al Localhost
    server.sin_port = htons(8080);//Conectarse al puerto 8888 
    cout << "Enlazando.." << endl;
    if(bind(socketDescription,(struct sockaddr *)&server, sizeof(server)) < 0){
        cout << "Error con el enlace" << endl;
        exit(1);
    }
    cout << "Enlace con Exito..." << endl;
    listen(socketDescription,3); //Listen para conectarse con el cliente
    cout << "Esperando conexiones" << endl;

    size = sizeof(struct sockaddr_in);
    while( (newSocket = accept(socketDescription, (struct sockaddr *)&client, (socklen_t*)&size)) )
    {
        cout << "Conexion aceptada" << endl;
        //Reply to the client
        message = "Mensaje de prueba - Kathia Barahona\n";
        write(newSocket , message.c_str() , message.length());
        pthread_t connectionThread;
        newSocketPointer  = (int *) malloc(1);
        *newSocketPointer = newSocket;
        pthread_create( &connectionThread , NULL ,  connectListenner , (void*) newSocketPointer);

    }//Multiples clientes pueden conectarse
    if(newSocket < 0){
        cout << "Conexion rechazada";
        exit(1);
    }
  
    return 0;
}
void *connectListenner(void *socketDescription){
    
    int socket = *(int*)socketDescription;
    char clientResponse[2000]; 
    string message;
    int buffer_size; 
    //Enviar mensajes
    message = "Hola, todo bien?\n";
    write(socket , message.c_str() , message.length());
     
    message = "Soy un hilo asignado, Adios";
    write(socket , message.c_str() , message.length());

    while((buffer_size = recv(socket, clientResponse, 2000, 0)) > 0){
        write(socket, clientResponse, strlen(clientResponse));
    }
    if(buffer_size == 0){
        cout << "Cliente desconectado ..." << endl;
    }else{
        if(buffer_size == -1){
            cout << "Ha ocurrido un error" << endl;
        }
    }
    //Free the socket pointer
    free(socketDescription);
     
    return 0;
}