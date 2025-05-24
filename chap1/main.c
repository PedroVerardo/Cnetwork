#include<sys/socket.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<stdio.h>
#include<stdlib.h>

int main(void){
  struct ifaddrs *addresses;

  if(getifaddrs(&addresses) == -1){ 
    printf("getifaddrs call failed\n");
    return -1;
  }

  struct ifaddrs *pointer = addresses;

  while(pointer){
    int family = pointer->ifa_addr->sa_family;
    if(family == AF_INET || family == AF_INET6){
      printf("%s\t", pointer->ifa_name);
      printf("%s\t", family == AF_INET ? "ipv4" : "ipv6");
      

      char ap[100];
      const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

      getnameinfo(pointer->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
      printf("\t%s\n", ap);
    }
    
    pointer = pointer->ifa_next;
  }
  
  freeifaddrs(addresses);
  return 0;
}
