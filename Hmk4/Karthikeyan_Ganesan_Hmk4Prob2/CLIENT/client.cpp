/*
Author Name : Karthikeyan Ganesan
Class : ECE 6122
Last Date Modified : 24th October 2019

Description : This file implements the UDP client which sends parts of a composite message to the server*/

///////////////////////////////////////////////////
/////Include the relevant header files////////////
/////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
// Include the headers for sockets //
#include <sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<unistd.h>
#include <fcntl.h>   //for creating non-blocking sockets
#include"udpMessage.h"
#include<thread>
#include<atomic>
/////////////////////////////////////////
/////Define Global variables////////////
///////////////////////////////////////

typedef int SOCKET;
using namespace std;
std::atomic<bool> kill (false);
///////////////////////////////////////////////////
/////Thread to receive messages from the server///
/////////////////////////////////////////////////

void receive_thread(SOCKET sockId, sockaddr_in serv_addr)
{
  struct udpMessage buffer;
  socklen_t servLen;
  servLen = sizeof(serv_addr);
  memset((char*)&buffer, 0, sizeof(buffer));
  while(!kill)
  {
    auto n = recvfrom(sockId,&buffer,sizeof(udpMessage),0,(struct sockaddr*)& serv_addr,&servLen);
    if (n > 0)
    {
      buffer.chMsg[buffer.nMsgLen] = 0;
      string g = buffer.chMsg;
      std::cout << "Received Msg Type:" << (int)buffer.nType<<","<<"Seq:"<<buffer.lSeqNum<<","<<"Msg:"<<g<<'\n';
    }
    receive_thread(sockId,serv_addr);
  }

  }
/////////////////////////////////////////////////////////////
/////Main Thread for interacting with the user inputs //////
////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
  int sockfd, portno, n;
  struct udpMessage msg;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  string user_ip;
  char assigner;
  socklen_t serverlen = 0;
  struct sockaddr from;
  memset(&from, 0, sizeof(sockaddr));
  if (argc < 3) {
    cout << "Enter hostname and port number" << '\n';
    exit (0) ;
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if (sockfd < 0)
        cout<<"Error opening socket"<<endl;

  server = gethostbyname(argv[1]);
  if (server == NULL)
  {
      cout<<"No such host"<<endl;
      exit(0);
  }
  serverlen = sizeof(serv_addr);
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memmove(&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
       cout<<"Error connecting"<<endl;

  thread receiver(receive_thread,sockfd,serv_addr);
  unsigned char nVersion = 0;
  while (true)
  {
    memset(&msg,0,sizeof(udpMessage));
    std::cout << "Please enter command" << '\n';
    getline(cin,user_ip);
    assigner = user_ip[0];
    switch (assigner)
    {
      case 'v':
      {
        size_t pos = user_ip.find(' ');
        nVersion = (unsigned char)stoi(user_ip.substr(pos+1));
        break;
      }
      case 't':
      {
        string command = user_ip.substr(2);
        size_t pos = command.substr().find(' ');
        // Update version number
        msg.nVersion = nVersion;
        // Update message type
        msg.nType = (unsigned char)stoi(command.substr(0,pos));

        command = command.substr(pos+1);
        // Extract the sequence number
        pos = command.substr().find(' ');
        msg.lSeqNum = (unsigned long)stol(command.substr(0,pos));

        command = command.substr(pos+1);
        msg.nMsgLen = (unsigned short) command.size();
        if (msg.nMsgLen > 1000)
        msg.nMsgLen = 1000;
        // Copy the message and update the message type
        memmove((char*)&msg.chMsg, command.c_str(), msg.nMsgLen);
        n = sendto(sockfd,(char*)&msg,sizeof(udpMessage),0,(struct sockaddr *) &serv_addr,serverlen);
        break;
      }
    }
    if (assigner == 'q')
    break;
  }

  // Kill all processes and threads and close the sockets
  kill = true;
  receiver.join();
  close(sockfd);

  return 0;
}
