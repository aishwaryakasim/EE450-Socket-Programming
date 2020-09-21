#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]){
    //boot up message 
    cout<<"The client is up and running\n";

    //command line inputs
    int snode,dnode,filesize;
    char map= *argv[1];
    snode = atoi(argv[2]);
    dnode = atoi(argv[3]);
    filesize = atoi(argv[4]);

    //create tcp client socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    //specify IP address and dynamic port for the tcp socket
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    //client_address.sin_port = htons(30000);
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len=sizeof(client_address);
    getsockname(client_socket,(struct sockaddr *) &client_address,&len);
    //cout<<"port no: "<<ntohs(client_address.sin_port);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(34321);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //connect to remote addr, aws here
    socklen_t len1=sizeof(server_address);
    int connection_status = connect(client_socket,(struct sockaddr *) &server_address, len1);
    //check for error within connection_status
    if(connection_status==-1){
        cout<<"there's an error \n";
        exit(0);
    }

    //send inputs to AWS
    send(client_socket, &map, sizeof(map),0);
    send(client_socket, &snode, sizeof(snode),0);
    send(client_socket, &dnode, sizeof(dnode),0);
    send(client_socket, &filesize, sizeof(filesize),0);
    cout<<"The client has sent query to AWS using TCP start vertex: "<< snode<< ", destination vertex: "<<dnode<< ", map id: " <<map<< ", file size: "<<filesize<<"\n";
    //listen(client_socket, 5);
//this_thread::sleep_for(std::chrono::milliseconds(200));

    char buffer[9999];
    int n=0;
    n=recv(client_socket, &buffer, sizeof(buffer), 0);
    buffer[n]='\0';
    string msg1=buffer;
//    cout<<"."<<endl;
    if(msg1!="NOTFOUND") {


        float short_dist = atof(msg1.c_str());

        n = recv(client_socket, &buffer, sizeof(buffer), 0);
        buffer[n] = '\0';

        float tx_delay = atof(buffer);

        n = recv(client_socket, &buffer, sizeof(buffer), 0);
        buffer[n] = '\0';
        float prop_delay = atof(buffer);

        n = recv(client_socket, &buffer, sizeof(buffer), 0);
        buffer[n] = '\0';
        string path = buffer;

        cout << setw(5) << "      The client has received results from AWS:" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "Source       Destination     Min Length    Tt   Tp     Delay" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << snode <<"     " << dnode <<"     " << "     " << short_dist<<"     " << tx_delay<<"     " << prop_delay<<"     " <<"     " <<
              prop_delay + tx_delay << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "Shortest Path    " << path << endl;
    } else{
        n = recv(client_socket, &buffer, sizeof(buffer), 0);
        buffer[n] = '\0';
        string error=buffer;
        if(error=="M")
            cout<<"No map id "<<map<<" found"<<endl;
        else
            cout<<"No vertex id "<<error<<" "<<" found"<<endl;

    }
    //receive final answer from AWS
    //char server_response[256];
    //recv(network_socket, &server_response,sizeof(server_response),0);

    // print out server's response
    //cout<<"server's response is: "<<server_response);
    

    //close the socket
    close(client_socket);
    return 0;
}
