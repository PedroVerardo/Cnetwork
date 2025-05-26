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

 // write socket first is a problem, its not clear why.

#include<stdio.h>
#include<time.h>
#include<string.h>


int main(void){
  printf("Configure local address...\n");
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints)); // Change all values of hints to zero
  hints.ai_family = AF_INET; //AF_INET = IPV4/ AF_INET6 = IPV6
  hints.ai_socktype = SOCK_STREAM; // SOCK_STREAM = TCP/ SOCK_DGRAM = UDP
  hints.ai_flags = AI_PASSIVE; // ?

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  printf("Creating socket on port 8080...\n");

  int socket_listening = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  
  if(socket_listening < 0){
    fprintf(stderr, "socket() failed. (%d)\n", errno);
    return 1;
  }

  printf("Binding socket to the local adress...\n");
  if(bind(socket_listening, bind_address->ai_addr, bind_address->ai_addrlen)){
    fprintf(stderr, "bind() failed. (%d)\n", errno);
    return 1;
  }

  printf("Listening...\n");
  if(listen(socket_listening, 10) < 0){ //listen have connection queue
    fprintf(stderr, "listen() failed. (%d)\n", errno);
    return 1;
  }

  printf("Waiting for connections...\n");
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);

  int socket_client = accept(socket_listening, (struct sockaddr *) &client_address, &client_len);

  if(socket_client < 0){
    fprintf(stderr, "accept() connection failed. (%d)\n", errno);
    return 1;
  }

  // just show connectioninformation
  printf("Client is connected...\n");
  char address_buffer[100];
  getnameinfo((struct sockaddr*) &client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);

  printf("%s\n", address_buffer);

  // defining the allocation for the request
  printf("Reading request...\n");
  char request[1024];

  int bytes_received = recv(socket_client, request, sizeof(request), 0);
  printf("Recieved %d bytes.\n", bytes_received);

  // the book made two sends?? the server make the packages separation to send?
  printf("Sending response...\n");
  const char *response = 
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Local time is: ";
  int bytes_send = send(socket_client, response, strlen(response), 0);
  printf("Sent %d of %d bytes.\n", bytes_send, (int)strlen(response));

  time_t timer;
  time(&timer);

  char *time_msg = ctime(&timer);
  bytes_send = send(socket_client, time_msg, strlen(time_msg), 0);
  printf("Sent %d of %d bytes.\n", bytes_send, (int)strlen(time_msg));

  printf("Closing connection...\n");
  close(socket_client);
  printf("Closing listenign socket...\n");
  close(socket_listening);

  return 0;
}
