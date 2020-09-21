#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "sstream"
#include <bits/stdc++.h>
using namespace std;
string *resultstring;
void  printPath( unordered_map<int,int> &parent, int j,string * s)
{

    // Base Case : If j is source
    if (parent[j] == - 1) {
        return ;
    }
    int i=parent[j];
    printPath(parent, i,s);

    *s=*s +"---"+to_string(j);

}
string * shortestPath(unordered_map< int, vector< pair<int, float> > > &adj, int V, int src, int dist, vector<int> &v)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // http://geeksquiz.com/implement-min-heap-using-stl/
    priority_queue< pair<int,float>, vector <pair<int,float>> , greater<pair<int,float>> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    unordered_map<int,float> distance(v.size());
    unordered_map<int,int> parent(v.size());



    for(int i=0;i<v.size();i++){
        distance[v[i]]=999999999;
        parent[v[i]]=-1;
    }



    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(make_pair( src,0));
    distance[src] = 0;

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().first;
        pq.pop();

        // Get all adjacent of u.
        for (auto x : adj.at(u))
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = x.first;
            float weight = x.second;

            // If there is shorted path to v through u.
            if (distance[v] > distance[u] + weight)
            {
                // Updating distance of v
                distance[v] = distance[u] + weight;
                pq.push(make_pair(v,distance[v]));
                parent[v]=u;
            }
        }
    }

    // Print shortest distances stored in dist[]
    //cout<<"shortest Distance of  source "<<src<<" to "<<" dst "<<dist<<" is "<<distance[dist]<<endl;
    string s;
    s=to_string(src);
    printPath(parent,dist,&s);
    string s1=to_string(distance[dist]);
    resultstring = new string [2];
    resultstring[0]=s1;
    resultstring[1]=s;
    return resultstring;
}



int main(){
    
    //create the Server C socket
    int serverC_socket;
    serverC_socket = socket(AF_INET, SOCK_DGRAM,0);

    //define the Server C address
    struct sockaddr_in serverC_address;
    serverC_address.sin_family = AF_INET;
    serverC_address.sin_port = htons(32321);
    serverC_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct sockaddr_in udp_address;
    udp_address.sin_family = AF_INET;
    udp_address.sin_port = htons(33321);
    udp_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //bind the socket to our specified IP and sin_port
    bind(serverC_socket, (struct sockaddr*) &serverC_address, sizeof(serverC_address));

    //boot up message
    cout<<"The Server C is up and running using UDP on port number: "<<ntohs(serverC_address.sin_port)<<"\n";

while(1){
    sleep(5);
    char buffer[999999];

    socklen_t addr_size;
    addr_size=sizeof(udp_address);

    int n=recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string src=buffer;
    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string dst=buffer;

   // cout<<"src "<<src<<" dst "<<dst<<endl;

    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string data=buffer;



    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string ch_txdelay=buffer;

    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string ch_tx_speed=buffer;

    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string ch_prop_speed=buffer;



    n =recvfrom(serverC_socket,&buffer,sizeof(buffer),0, (struct sockaddr*)&udp_address, &addr_size);
    buffer[n]='\0';
    string id=buffer;
    cout<<"The Server C has received data for calculation:"<<endl;
    cout<<"\t \t * Propagation speed: "<<ch_prop_speed<<" km/s;"<<endl;
    cout<<"\t \t * Transmission speed: "<<ch_tx_speed<<" KB/s;"<<endl;
    cout<<"\t \t * map ID: "<<id<<" ;"<<endl;
    cout<<"\t \t * Source ID  "<<src<<"     Destination ID "<<dst<<" ;"<<endl;


    int q_source,q_dest;
    q_source=atoi(src.c_str());
    q_dest=atoi(dst.c_str());

    stringstream ss;

    /* Storing the whole string into string stream */
    vector<int> v ;
    vector<float > val ;

    ss << data;

    /* Running loop till the end of the stream */
    string temp;
    float found;

    int cc=2;
    while (!ss.eof()) {

        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */

            if (cc != 0) {

                if (stringstream(temp) >> found) {
                    cc--;
                    if (std::find(v.begin(), v.end(), found) != v.end())
                        continue;
                    else
                        v.push_back(found);
                }

                temp = "";
            } else{
                temp="";
                cc = 2;
            }}
    unordered_map< int, vector< pair<int, float> > > adj(2*v.size()+1);
    stringstream sss;
    sss << data;


    int source;
    int dest;
    cc=2;

    while (!sss.eof()) {

        /* extracting word by word from stream */
        sss >> temp;

        /* Checking the given word is integer or not */
            if (stringstream(temp) >> found) {
                if (cc == 2) {
                    cc--;
                    source=(int)found;}
                else if(cc==1){
                    cc--;
                    dest=(int)found;
                }
                else{

                    adj[source].push_back(make_pair(dest,found));
                    adj[dest].push_back(make_pair(source,found));

                    cc=2;
                }


            }
            temp = "";
        }


    string *result;
    result=shortestPath(adj,v.size(),q_source,q_dest,v);
    float short_dist=atof(result[0].c_str());
    string path=result[1];

    //float short_dist=shortestPath(adj,v.size(),q_source,q_dest,v);
    //cout<<"*************"<<endl;
    //cout<<"Short distance is "<<short_dist<<endl;
    //cout<<"Shortest Path is "<<path<<endl;





    float prop_speed=atof(ch_prop_speed.c_str());



    char ch_path[path.size()];
    strcpy(ch_path, path.c_str());

    float prop_delay=short_dist/prop_speed;

    cout<<"The Server C has finished the calculation:"<<endl;
    cout<<"Shortest Path :"<<path<<endl;
    cout<<"Shortest Distance :"<<short_dist<<" km" <<endl;
    cout<<"Transmission Delay :"<<ch_txdelay<<" s"<<endl;
    cout<<"Propagation Delay:"<<prop_delay<<" s"<<endl;


    char ch_prop_delay[to_string(prop_delay).size()];
    strcpy(ch_prop_delay,to_string(prop_delay).c_str());

    char ch_shortdist[to_string(short_dist).size()];
    strcpy(ch_shortdist,to_string(short_dist).c_str());

    sendto(serverC_socket, (const char *) ch_path, sizeof(ch_path), 0, (struct sockaddr *) &udp_address,
           sizeof(udp_address));
    sendto(serverC_socket, (const char *) ch_prop_delay, sizeof(ch_prop_delay), 0, (struct sockaddr *) &udp_address,
           sizeof(udp_address));

    sendto(serverC_socket, (const char *) ch_shortdist, sizeof(ch_shortdist), 0, (struct sockaddr *) &udp_address,
           sizeof(udp_address));

    cout<<" The Server C has finished sending the output to AWS"<<endl;

    delete [] resultstring;

}



    return 0;
}
