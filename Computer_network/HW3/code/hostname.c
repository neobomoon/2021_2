#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int
main ( int argc, char* argv[] ) {
    if ( argc != 2 ) {
        printf("Usage: %s <URL>\n", argv[0]);
		exit(1);    
    }
    struct in_addr addr ;
    struct hostent *host_entry ;
    host_entry = gethostbyname( argv[1] ) ;

    if ( host_entry == 0 ) {
        printf( "fail to gethostbyname()\n" ) ;
        exit( 1 ) ;
    }
    printf( "Official name: %s\n", host_entry->h_name ) ;
    int i ;
    for ( i = 0 ; host_entry->h_aliases[i] != NULL ; i++ ) {
        printf( "Aliases %d: %s\n", i + 1, host_entry->h_aliases[i]) ;
    }
    printf( "Address type: %s\n", host_entry->h_addrtype == 2 ? "AF_INET" : "AF_INET6" ) ;
    for ( i = 0 ; host_entry->h_addr_list[i] != NULL ; i++ ) {
        addr.s_addr = *(long int *)host_entry->h_addr_list[i] ;
        printf( "IP addr %i: %s\n", i + 1,inet_ntoa(addr)) ;
    }


    return 0 ;
}