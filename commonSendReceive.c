#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

int sendTCP(int sock, char* msg, int sizeMsg) {
  ssize_t snd;
  //int total_sent = 0;
  while(sizeMsg > 0) {
    snd = send(sock, msg, sizeMsg, 0);
    if (snd < 0) {
      return -1;
    } 
    if (snd == 0) {
      return 0;
    }

  printf("Client : j'ai déposé %d octets \n", (int) snd);

    //total_sent += snd;
    sizeMsg -= snd;
  }
  return 1;
}

int recvTCP(int sock, char * msg, int sizeMsg) {
  ssize_t rcv;
  //int total_rcv = 0;
  while(sizeMsg > 0) {
    rcv = recv(sock, msg, sizeMsg, 0);
    if (rcv < 0) {
      return -1;
    } 
    if (rcv == 0) {
      return 0;
    }

    //printf("Client : j'ai recu %d octets \n", (int) rcv);

    //total_rcv += rcv;
    sizeMsg -= rcv;
  }
  return 1;
}