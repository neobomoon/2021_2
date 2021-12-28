#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int 
main ( int argc, char *argv[] ) {
    if ( argc != 2 ) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

    int serv_sd ;
    if ( ( serv_sd = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
        printf( "fail to generate socket\n" ) ;
        exit( 1 ) ;
    }

    struct sockaddr_in serv_addr ;
    memset( &serv_addr, 0, sizeof( serv_addr ) ) ;
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY ) ;
    serv_addr.sin_port = htons( atoi( argv[ 1 ] ) ) ;

    bind( serv_sd, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) ;
    listen( serv_sd, 5 ) ;


    while( 1 ) {

        struct sockaddr_in client_addr ;
        socklen_t client_addr_size = sizeof( client_addr ) ;
        int read_client ;
        char buf[BUF_SIZE] ;
        memset( buf, 0, BUF_SIZE ) ;

        printf( "* wait to connect to client\n" ) ;

        int client_sd = accept( serv_sd, ( struct sockaddr* )&client_addr, &client_addr_size ) ;
        printf( "* success to connect to client\n\n" ) ;
        
        // receive file_len
        int check ;
        int file_len ;
        check = recv( client_sd, &file_len, sizeof(int), 0 ) ;
        if ( check < -1 ) {
            close( client_sd ) ;
            exit( 1 ) ;
        }
        // printf("file len: %d\n", file_len) ;

        // receive filename
        char* filename = malloc( sizeof(char) * file_len ) ;
        check = recv( client_sd, buf, file_len, 0 ) ;
        if ( check < -1 ) {
            close( client_sd ) ;
            exit( 1 ) ;
        }
        strcpy(filename, buf) ;
        printf("filename: %s\n", filename) ;

        // open file
        FILE *fp ;
        if ( ( fp = fopen( filename, "wb" ) ) == NULL ) {
            printf( "fail to open file\n" ) ;
            exit( 1 ) ;
        }
        memset( buf, 0, BUF_SIZE ) ;
        // receive filedata
        int read_cnt ;
        int pkt_cnt = 0 ;
        while( ( read_cnt = recv( client_sd, buf, BUF_SIZE, 0 ) ) != 0 ) {
            // write file
            // printf("read: %d count: %d len: %d\n", read_cnt, pkt_cnt, strlen(buf)) ;
            printf("%d ", read_cnt) ;
            pkt_cnt++ ;
            fwrite( (void*)buf, 1, read_cnt, fp ) ;
            memset( buf, 0, BUF_SIZE ) ;
        }
        printf( "\n%d packets arrived\n", pkt_cnt ) ;
        printf( "completed file transmit!\n--------------------------\n\n" ) ;
        fclose( fp ) ;
        shutdown( client_sd, SHUT_WR ) ;
        close( client_sd ) ;
        free( filename ) ;

    }

    printf( "terminate the server\n" ) ;

    close(serv_sd);
    return 0 ;
}