/*
Author Name : Karthikeyan Ganesan
Class : ECE 6122
Last Date Modified : 24th October 2019

Description : This file implements the UDP server for a single server - multiple client architectures.
This server recieves parts of a composite mesage from multiple clients and composes the entire message
This server also manipulates the composed message based on various conditions*/
///////////////////////////////////////////////////
/////Include the relevant header files////////////
/////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
// Include the headers for sockets //
#include <sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include<unistd.h>
#include <fcntl.h>   //for creating non-blocking sockets
#include"udpMessage.h"
#include<thread>
#include<mutex>
#include<vector>
#include<map>
#include<algorithm>
#include<atomic>
/////////////////////////////////////////
/////Define Global variables////////////
///////////////////////////////////////

typedef int SOCKET;
using namespace std;
mutex mtx;
std::map<unsigned long,string> composite_message;
std::vector<sockaddr_in> client_list;
int client_count = 0 ;
std::atomic<bool> kill (false);
int mes_length = 1 ;
///////////////////////////////////////////////////
/////Thread to handle various functionalities/////
/////////////////////////////////////////////////

void handle_ip_message(udpMessage &buff, SOCKET sockId)
{
  if (buff.nVersion == 1)
  {
    //buff.chMsg[buff.nMsgLen] = 0;
    if (buff.nType == 0)
    {
      mtx.lock();
      composite_message.clear();
      mtx.unlock();
    }
    else if (buff.nType == 1)
    {
      mtx.lock();
      composite_message.clear();
      string mess = buff.chMsg;  //convert character message to string
      composite_message.insert(pair<unsigned long,string> (0,mess));
      mes_length = 1;
      mtx.unlock();
    }
    else if (buff.nType == 2)
    {
      mtx.lock();
      string mess = buff.chMsg;    //convert character message to string
      composite_message.insert(pair<unsigned long,string>(buff.lSeqNum,mess));
      mes_length ++ ;
      mtx.unlock();
    }
    else if (buff.nType == 3)
    {
      sockaddr_in client_addr;
      udpMessage msg;
      unsigned long seqNum = 1;
      msg.nVersion = 1;
      msg.nType = 1;
      string mess = buff.chMsg;  //convert character message to string
      composite_message.insert(pair<unsigned long,string> (buff.lSeqNum,mess));
      string constructed_message;
      mtx.lock();
      //convert composite message map to string
       for (auto& msg : composite_message)
       {
           constructed_message += msg.second;
       }
       msg.nMsgLen = constructed_message.size();

      memmove(&msg.chMsg, constructed_message.c_str(), constructed_message.size());
      for (auto it = client_list.begin();it!= client_list.end();it++)
      {
        memset(&client_addr, 0, sizeof(client_addr));
        client_addr = *it;
        msg.lSeqNum = seqNum;
        auto n = sendto(sockId,&msg,sizeof(udpMessage),0,(struct sockaddr*)&client_addr, sizeof(client_addr));
        if (n < 0)
        {
          std::cout << "Error sending message" << '\n';
        }
        seqNum ++ ;
      }
      composite_message.clear();
      mtx.unlock();
    }
    else if (mes_length > 1000)
    {
      sockaddr_in client_addr;
      udpMessage msg;
      unsigned long seqNum = 1;
      string constructed_message;
      msg.nVersion = 1;
      msg.nType = 1;
      for (auto msg : composite_message)
      {
          constructed_message += msg.second;
      }
      // message has been constructed in string format
      char *actual_msg = new char[constructed_message.length() + 1];
      int residual_length = constructed_message.length() - 1000;
      char required[1000],residual[residual_length]; //variables to store the required and residual part
      strcpy(actual_msg,constructed_message.c_str());
      int k = 0 ;
      for (int i = 0;i<1000;i++)
      {
        required[i] = actual_msg[i];
      }
      for (int i = 1000 ; i < residual_length; i++)
      {
        residual[k] = actual_msg[i];
        k++;
      }
     memmove(&msg.chMsg,required,constructed_message.size());
     for (auto it = client_list.begin();it!= client_list.end();it++)
     {
       memset(&client_addr, 0, sizeof(client_addr));
       client_addr = *it;
       msg.lSeqNum = seqNum;
       auto n = sendto(sockId,&msg,sizeof(udpMessage),0,(struct sockaddr*)&client_addr, sizeof(client_addr));
       if (n < 0)
       {
         std::cout << "Error sending message" << '\n';
       }
       seqNum ++ ;
     }
     string res_mess = residual;
     composite_message.clear();
     composite_message.insert(pair<unsigned long,string> (0,res_mess));
    }
  }
}

/////////////////////////////////////////////////////////////
/////Thread to continously receive messages from clients/////
////////////////////////////////////////////////////////////

void communicate(SOCKET sockId)
{
  int n;
  struct sockaddr_in client;
  socklen_t clientLen;
  struct udpMessage buffer;
  clientLen = sizeof(client);
  while(!kill)
  {
    memset(&buffer,0, sizeof(buffer));
    n = recvfrom(sockId,&buffer,sizeof(udpMessage),0,(struct sockaddr *) &client, &clientLen);
    if (n < 0)
    {
      std::cout << "Error receiving" << '\n';
    }
    handle_ip_message(buffer,sockId);
    // add client to client list
    auto client_addr = client;
    mtx.lock();
    auto it = find_if(client_list.begin(),client_list.end(), [&client_addr](struct sockaddr_in &s){ return (s.sin_addr.s_addr == client_addr.sin_addr.s_addr)  && (s.sin_port == client_addr.sin_port); });
    if (it == client_list.end())
    client_list.push_back(client_addr);
    client_count ++ ;
    mtx.unlock();
  }
}

//////////////////////////////////////////
/////Functions to handle user inputs/////
////////////////////////////////////////

void first_action(SOCKET sockId)
{
  sockaddr_in client_addr;
  udpMessage msg;
  static unsigned long seqNum = 1;
  msg.nVersion = 1;
  msg.nType = 1;
  string constructed_message;
  mtx.lock();
  //convert composite message map to string
   for (auto& msg : composite_message)
   {
       constructed_message += msg.second;
   }
  memmove(&msg.chMsg, constructed_message.c_str(), constructed_message.size());
  msg.nMsgLen = constructed_message.size();
  for (auto it = client_list.begin();it!= client_list.end();it++)
  {
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr = *it;
    msg.lSeqNum = seqNum;
    auto n = sendto(sockId,&msg,sizeof(udpMessage),0,(struct sockaddr*)&client_addr, sizeof(client_addr));
    if (n < 0)
    {
      std::cout << "Error sending message" << '\n';
    }
  }
  composite_message.clear();
  seqNum++;
  mtx.unlock();
}

void second_action()
{
  mtx.lock();
  composite_message.clear();
  mtx.unlock();
}
void third_action()
{
  cout<<"Composite Msg:"<<"\t";
  mtx.lock();
  for (auto& it: composite_message)
  {
    cout<<it.second;
  }
  cout<<endl;
  mtx.unlock();
}

/////////////////////////////////////////////////////////////
/////Main Thread for interacting with the user inputs //////
////////////////////////////////////////////////////////////

int  main(int argc, char const *argv[])
{
  int sockId1,portNo;
  struct sockaddr_in server;
  int command;
  // check for number of arguments
  if (argc < 2)
  {
    cout << "Execute along with port number" << '\n';
    exit(0);
  }
  // assign input port number
  portNo = atoi(argv[1]);
  // Create a socket
  sockId1 = socket(AF_INET,SOCK_DGRAM,0);
  if (sockId1 < 0)
  {
    cout << "Error creating socket" << '\n';
  }

  // set server variable to zero value and initialize the internet address
  memset(&server,0,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(portNo);
  server.sin_addr.s_addr = INADDR_ANY;
  // Bind socket to port
  if (bind(sockId1, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        std::cout << "Error on binding" << '\n';
        exit(0);
    }
  thread th1(communicate,sockId1);
  // Now start communicating with clients and accept user inputs
  while (true)
     {
         // Deal with various user inputs
         cout << "Please enter command";
         cin >> command;
         switch (command)
         {
           // First user command
           case 0:
           first_action(sockId1);
           break;
           // Second user command
           case 1:
           second_action();
           break;
           // third user command
           case 2:
           third_action();
           break;
         }

      }

// Kill all processes and threads and close the sockets
kill = true;
th1.join();
close(sockId1);

return 0;
}
