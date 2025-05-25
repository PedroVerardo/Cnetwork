#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
/*
 * 1- What is a socket?
 *  sockets are IPC that could be used on networks.
 *  A socket is defined by a port number, a IP adress and a protocol(tcp/udp).
 *  When someone talking about sockets normally they talk about berkely sockets
 *  that are invented by the university of berkely.*/

#include<stdio.h>
#include<time.h>
#include<string.h>


int main(void){
  time_t *timer;
  time(timer);

  printf("local time is: %s\n", ctime(timer));

  printf("Configure local address...\n");
  struct addrinfo hints;

  menset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  return 0;
}
