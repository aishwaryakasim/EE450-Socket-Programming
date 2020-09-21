#include <iostream>
#include <stdlib.h>
#include <cctype>
#include <cstdlib>
#include <string>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int main(){
    
    //create the Server B socket
    int serverB_socket;
    serverB_socket = socket(AF_INET, SOCK_DGRAM,0);

    //define the Server B address
    struct sockaddr_in serverB_address, udp_address;
    serverB_address.sin_family = AF_INET;
    serverB_address.sin_port = htons(31321);
    serverB_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //bind the socket to our specified IP and sin_port
    bind(serverB_socket, (struct sockaddr*) &serverB_address, sizeof(serverB_address));
    
    //boot up message
    cout<<"The Server B is up and running using UDP on port number: "<<ntohs(serverB_address.sin_port)<<"\n";

    //receive the map id from aws
    char maprecaws;
    while(1)
    {
    socklen_t addr_size; 
    addr_size=sizeof(udp_address);
    recvfrom(serverB_socket,&maprecaws,sizeof(maprecaws),0, (struct sockaddr*)&udp_address, &addr_size);
    if(!isalpha(maprecaws))
        continue;
    
    cout<<"The Server B has received input for finding graph of map: "<<maprecaws<<"\n";
    
    //convert char map id to string
    string id;
    id=maprecaws;
    
    //open map1.txt file and search
    ifstream myFile;
    myFile.open("map2.txt");  
    
    //if cannot open, throw error
    if(!myFile){
        cerr<<"Unable to open map2.txt\n";
        exit(1);}
    
    string content,line,propagation_speed,transmission_speed;
    int found_flag=0;
    //read loop
    while(getline(myFile,line)){
        
        if(line==id){
            getline(myFile,line);
            propagation_speed=line;
            //cout<<"propation speed is: "<<propagation_speed<<"\n";//<<prop_speed<<"\n";
            
            getline(myFile,line);
            transmission_speed=line;
            //cout<<"transmission speed is: "<<transmission_speed<<"\n";//<<tx_speed<<"\n";
            
            while(getline(myFile,line)){
                        if(!isdigit(line.c_str()[0])){break;cout<<"not found";}  
                        //cout<<line<<"\n";
                        content.append(line);
                        content.append("\n");
            }//close while, read nodes

            //converting string to char* to sendto()
            int n=propagation_speed.length();
            char prop_speed[n];
            strcpy(prop_speed,propagation_speed.c_str());

            n=transmission_speed.length();
            char tx_speed[n];
            strcpy(tx_speed,transmission_speed.c_str());
            
            n=content.length();
            char cont[n];
            strcpy(cont,content.c_str());

            sendto(serverB_socket, (const char *)prop_speed, strlen(prop_speed),0,(const struct sockaddr*)&udp_address, sizeof(udp_address));
            sendto(serverB_socket, (const char *)tx_speed, strlen(tx_speed),0,(const struct sockaddr*)&udp_address, sizeof(udp_address));
            sendto(serverB_socket, (const char *)cont, sizeof(cont),0,(struct sockaddr*)&udp_address, sizeof(udp_address));
            cout<<"The Server B has sent Graph to AWS \n";
            found_flag=1;
            break;
        }//close if, read id,prop,tx speeds 
    
    //else if(line!=id)
    //cout<<"The server B does not have the required map id"<<maprecaws;
    
    }//close while, read loop
    if(found_flag!=1) {
        cout << "The server B does not have the required map id" << endl;
        cout<<"The Server B has sent \"Graph not Found\" to AWS"<<endl;
        char notfound[] = {'N', 'O', 'T', 'F', 'O', 'U', 'N', 'D'};
        sendto(serverB_socket, (const char *) notfound, strlen(notfound), 0, (const struct sockaddr *) &udp_address,
               sizeof(udp_address));
        sendto(serverB_socket, (const char *) notfound, strlen(notfound), 0, (const struct sockaddr *) &udp_address,
               sizeof(udp_address));
        sendto(serverB_socket, (const char *) notfound, sizeof(notfound), 0, (struct sockaddr *) &udp_address,
               sizeof(udp_address));
    }

    myFile.close();
    maprecaws='1';
    
    
    }//end while(1)




    //close the sockaddr
    close(serverB_socket);

    return 0;
}