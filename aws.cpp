#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;
#define MAXLINE 1024



int main() {
    //1) boot up message
    cout << "The AWS server is up and running.\n";

    //CREATE AWS TCP, UDP SOCKETS FOR AWS
    int server_socket, toAB_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    toAB_socket = socket(AF_INET, SOCK_DGRAM, 0);

    //AWS TCP ADDRESS
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(34321);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");


    //bind
    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));


    //listen for tcp
    listen(server_socket, 5);

    //continuously run the server
    while (1) {

        //AWS UDP ADDRESS
        struct sockaddr_in udp_address;
        udp_address.sin_family = AF_INET;
        udp_address.sin_port = htons(33321);
        udp_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        //SERVER A UDP ADDRESS
        struct sockaddr_in serverA_address;
        serverA_address.sin_family = AF_INET;
        serverA_address.sin_port = htons(30321);
        serverA_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        //SERVER B UDP ADDRESS
        struct sockaddr_in serverB_address;
        serverB_address.sin_family = AF_INET;
        serverB_address.sin_port = htons(31321);
        serverB_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        //SERVER C UDP ADDRESS
        struct sockaddr_in serverC_address;
        serverC_address.sin_family = AF_INET;
        serverC_address.sin_port = htons(32321);
        serverC_address.sin_addr.s_addr = inet_addr("127.0.0.1");
        bind(toAB_socket, (struct sockaddr *) &udp_address, sizeof(udp_address));

        int client_socket=0, snode=0, dnode=0, filesize=0;
        char maprec='\0';
        socklen_t len = sizeof(client_address);
        socklen_t addrA_size = sizeof(serverA_address);
        socklen_t addrB_size = sizeof(serverB_address);

        //accept
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address,
                                 (socklen_t*)&client_address))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        //client_socket = accept(server_socket, (struct sockaddr *) &client_address, &len);

        //receive the inputs from client
        recv(client_socket, &maprec, sizeof(maprec), 0);
        recv(client_socket, &snode, sizeof(snode), 0);
        recv(client_socket, &dnode, sizeof(dnode), 0);
        recv(client_socket, &filesize, sizeof(filesize), 0);

        //2) print out that AWS rx client response
        cout << "The AWS has received map id: " << maprec << ", source vertex: " << snode << ", destination vertex: "
             << dnode << ", file size: " << filesize << ", from the client using TCP over port: "
             << ntohs(server_address.sin_port) << "\n";

        //3) send map id to server A
        sendto(toAB_socket, &maprec, sizeof(maprec), 0, (struct sockaddr *) &serverA_address, sizeof(serverA_address));
        cout << "The AWS has sent map ID to server A using UDP over port:" << ntohs(udp_address.sin_port) << "\n";

        //4) send map id to server B
        sendto(toAB_socket, &maprec, sizeof(maprec), 0, (struct sockaddr *) &serverB_address, sizeof(serverB_address));
        cout << "The AWS has sent map ID to server B using UDP over port: " << ntohs(udp_address.sin_port) << "\n";


        //Rx map data from A or B

        char buffer2[MAXLINE];

        int n = 0, port = 0, flag = 6;
        int bcount = 0;
        int acount = 0;

        string msgA[3];
        string msgB[3];
        string prop_speed;
        string tx_speed;
        string data ;

        while (flag != 0) {
            n = recvfrom(toAB_socket, (char *) buffer2, sizeof(buffer2), 0, (struct sockaddr *) &serverA_address,
                         &addrA_size);
            buffer2[n] = '\0';
            port = ntohs(serverA_address.sin_port);
            if (port == 31321) {
                msgB[bcount] = buffer2;
                bcount++;
            } else {
                msgA[acount] = buffer2;
                acount++;
            }
            flag--;
        }
        if (msgA[0] == "NOTFOUND" and msgB[0] == "NOTFOUND") {
            cout << "Map ID not found in both Server A and B.";
            cout << "\n";
            char notfound[] = {'N', 'O', 'T', 'F', 'O', 'U', 'N', 'D'};
            send(client_socket,  &notfound, sizeof(notfound), 0);
		 this_thread::sleep_for(std::chrono::milliseconds(5000));
            char error_code='M';
            send(client_socket, &(error_code), sizeof(error_code),0);

            continue;

        } else if (msgA[0] == "NOTFOUND" and msgB[0] != "NOTFOUND") {
            cout << "The AWS has received map information from server B "<<endl;
            prop_speed = msgB[0];
            tx_speed = msgB[1];
            data = msgB[2];

        } else {
            cout << "The AWS has received map information from server A"<<endl;
            prop_speed = msgA[0];
            tx_speed = msgA[1];
            data = msgA[2];

        }

        stringstream ss;
        vector<int> v;

        ss << data;
        string temp;
        int found;
        int cc = 2;
        while (!ss.eof()) {

            //extracting word by word from stream
            ss >> temp;

            //Checking the given word is integer or not

            if (cc != 0) {

                if (stringstream(temp) >> found) {
                    cc--;
                    if (std::find(v.begin(), v.end(), found) != v.end())
                        continue;
                    else
                        v.push_back(found);
                }

                temp = "";
            } else {
                cc = 2;
                temp = "";
            }
        }
        if ((std::find(v.begin(), v.end(), snode) != v.end()) && (std::find(v.begin(), v.end(), dnode) != v.end())) {
            cout << "The source and destination vertex are in the graph." << "\n";
            string temp = to_string(dnode);
            char ch_dnode[temp.size()];
            strcpy(ch_dnode, temp.c_str());

            temp = to_string(snode);
            char ch_snode[temp.size()];
            strcpy(ch_snode, temp.c_str());

            char ch_data[data.size()];
            strcpy(ch_data, data.c_str());

            //sending source node to c
            sendto(toAB_socket, &ch_snode, sizeof(ch_snode), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));
            //sending dnode to c
            sendto(toAB_socket, &ch_dnode, sizeof(ch_dnode), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));
            //sending data to c
            sendto(toAB_socket, &ch_data, sizeof(ch_data), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));

            //calculating tx delay in aws
            float tx_delay = filesize / atof(tx_speed.c_str());
            temp = to_string(tx_delay);
            char ch_txdelay[temp.size()];
            strcpy(ch_txdelay, temp.c_str());

            //sending tx delay to c
            sendto(toAB_socket, &ch_txdelay, sizeof(ch_txdelay), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));

            char ch_txspeed[tx_speed.size()];
            strcpy(ch_txspeed, tx_speed.c_str());
            // sending tx speed to c
            sendto(toAB_socket, &ch_txspeed, sizeof(ch_txspeed), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));



            char ch_prop_speed[prop_speed.size()];
            strcpy(ch_prop_speed,prop_speed.c_str());
            //sending prob speed to c
            sendto(toAB_socket, &ch_prop_speed, sizeof(ch_prop_speed), 0, (struct sockaddr *) &serverC_address,
                    sizeof(serverC_address));
            //sending map id to c
            sendto(toAB_socket, &maprec, sizeof(maprec), 0, (struct sockaddr *) &serverC_address,
                   sizeof(serverC_address));


            cout<<"The AWS has sent map, source ID, destination ID,propagation speed and transmission speed to server C using UDP over port "<<ntohs(udp_address.sin_port)<<endl;

            //get path from c
            n = recvfrom(toAB_socket, (char *) buffer2, sizeof(buffer2), 0, (struct sockaddr *) &serverA_address,
                         &addrA_size);
            buffer2[n] = '\0';
            string path = buffer2;

            //get prop delay form c
            n = recvfrom(toAB_socket, (char *) buffer2, sizeof(buffer2), 0, (struct sockaddr *) &serverA_address,
                         &addrA_size);
            buffer2[n] = '\0';
            string prop_delay =buffer2;

            //get shortest distance from c
            n = recvfrom(toAB_socket, (char *) buffer2, sizeof(buffer2), 0, (struct sockaddr *) &serverA_address,
                         &addrA_size);
            buffer2[n] = '\0';
            string short_dist =buffer2;

            cout<<"The AWS has received results from server C:"<<endl;
            cout<<"Shortest Path :"<<path<<endl;
            cout<<"Shortest Distance :"<<short_dist<<endl;
            cout<<"Transmission Delay :"<<tx_delay<<endl;
            cout<<"Propagation Delay:"<<prop_delay<<endl;

            cout<<"The AWS has sent calculated results to client using TCP over port "<<ntohs(server_address.sin_port)<<endl;

            char ch_shortdist[short_dist.size()];
            strcpy(ch_shortdist, short_dist.c_str());

            //send shortest distance to client
            cout<<"msg1"<<endl;

            int sb =send(client_socket, &(ch_shortdist), sizeof(ch_shortdist),0);
       //     cout<<sb;
		this_thread::sleep_for(std::chrono::milliseconds(3000));
         //send tx delay to client
            cout<<"msg2"<<endl;

            send(client_socket, &(ch_txdelay), sizeof(ch_txdelay),0);
	this_thread::sleep_for(std::chrono::milliseconds(3000));
            cout<<"msg3"<<endl;

            //send prop delay to client
            char ch_prop_delay[prop_delay.size()];
            strcpy(ch_prop_delay, prop_delay.c_str());
            send(client_socket, &(ch_prop_delay), sizeof(ch_prop_delay),0);
		this_thread::sleep_for(std::chrono::milliseconds(3000));
            cout<<"msg4"<<endl;

            //send path to client
            char ch_path[path.size()];
            strcpy(ch_path, path.c_str());
            send(client_socket, &(ch_path), sizeof(ch_path),0);
cout<<"done"<<endl;
        }
        else
            {
            char notfound[] = {'N', 'O', 'T', 'F', 'O', 'U', 'N', 'D'};
            send(client_socket,  &notfound, sizeof(notfound), 0);
		 this_thread::sleep_for(std::chrono::milliseconds(3000));
            if(not(std::find(v.begin(), v.end(), snode) != v.end())) {
                cout << "Source vertex not found in the graph, sending error to client using TCP over port "<<ntohs(server_address.sin_port)<< "\n";
                char error_code=(char) snode;
                send(client_socket, &(error_code), sizeof(error_code),0);
            }
                else {
                cout << "<Destination vertex not found in the graph, sending error to client using TCP over port"<<ntohs(server_address.sin_port)<< "\n";
                char error_code=(char) dnode;
                send(client_socket, &(error_code), sizeof(error_code),0);
            }
        }

    }//end while

    //close the sockets
    close(server_socket);
    close(toAB_socket);
    return 0;
}


