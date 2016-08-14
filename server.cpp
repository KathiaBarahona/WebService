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
#include <ctime>
#include <pthread.h>

using namespace std;

int socketDescription, newSocket, size, *newSocketPointer;
struct sockaddr_in server,client;
int port = 80;
string getContentType(const char*);
string getFileExt(const string&);
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
    char * tokens;
    string fileRoute;
    string status;
    string type;
    string body;
    string dateT;
    string head;
    string length;
    string extension;
    string serverText = "Server: localhost:8080 \r\n";
    string closeText = "COnnection: close\r\n";
    //Enviar mensajes


    while((buffer_size = recv(socket, clientResponse, 2000, 0)) > 0){
        
        tokens = strtok(clientResponse," ");
        if(strcmp(tokens,"GET") == 0){
            
            tokens = strtok(NULL, " ");
            fileRoute = tokens;
            fileRoute = fileRoute.substr(1);
            ifstream htmlFile (fileRoute.c_str(),ios::binary );

            tokens = strtok(NULL, " ");
            if (htmlFile.is_open()){
                if(strcmp(tokens,"HTTP/1.1")){
                    status = "HTTP/1.1 200 OK\r\n";
                }else{
                    status = "HTTP/1.0 200 OK\r\n";
                }
                
                time_t now = time(0);

                tm *ltm = localtime(&now);
                stringstream time;
                time << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
                time << " " << 1 + ltm->tm_hour <<":"<<1 + ltm->tm_min<<":"<< 1 + ltm->tm_sec;
                dateT = "Date: "+time.str()+"\r\n";
                extension = getFileExt(fileRoute);
                type = getContentType(extension.c_str());
                
              
                htmlFile.seekg( 0, ios::end );
                head = status+closeText+dateT+serverText;
                stringstream ss; 
                ss << "Content-Length: " << htmlFile.tellg() << "\r\n";
                length = ss.str();
                head += length + type;
                cout << head << endl;
                send(socket, head.c_str(),head.length(),0);
                htmlFile.close();
            }else{
                if(strcmp(tokens,"HTTP/1.1")){
                    status = "HTTP/1.1 404 not found\r\n";
                }else{
                    status = "HTTP/1.0 404 not found\r\n";
                }
            }

  
            cout << tokens << 1 << endl;
           

        }else{
            if (tokens == "POST"){

            }else{
                if (tokens == "PUT"){

                }else{
                    cout << "Ha ocurrido un error" << endl;
                } 
            }
        }
            
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
string getContentType( const char* extension){
    if(strcmp(extension,"html") || strcmp(extension,"txt") ){
        return "Content-Type: text/html\r\n\r\n";
    }
    if(strcmp(extension,"css")){
        return "Content-Type: text/css\r\n\r\n";
    }
    if(strcmp(extension,"jpeg") || strcmp(extension,"jpg")){
        return "Content-Type: image/jpeg\r\n\r\n";
    }
    if(strcmp(extension,"gif")){
        return "Content-Type: image/gif\r\n\r\n";
    }
    if(strcmp(extension,"png")){
        return "Content-Type: image/png\r\n\r\n";

    }
    return "Content-Type not found";
  
}
string getFileExt(const string& s) {

   size_t i = s.rfind('.', s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}