#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void
error_handling( char* message ) {
    fputs( message, stderr) ;
    fputc( '\n', stderr ) ;
    exit( 1 ) ;
}

int 
main ( int argc, char *argv[] ) {
    if ( argc != 2 ) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

    int serv_sd ;
    if ( ( serv_sd = socket( PF_INET, SOCK_DGRAM, 0 ) ) == -1 ) {
        printf( "fail to generate socket\n" ) ;
        exit( 1 ) ;
    }

    struct sockaddr_in serv_addr ;
    memset( &serv_addr, 0, sizeof( serv_addr ) ) ;
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY ) ;
    serv_addr.sin_port = htons( atoi( argv[ 1 ] ) ) ;

    if ( bind( serv_sd, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) == -1 ) {
        error_handling( "bind() error" ) ;
    }

    // file transmit
    struct sockaddr_in clnt_addr ;
    int clnt_addr_size ;
    char buffer[BUF_SIZE] = {0, } ;
    while( 1 ) {
        memset( &clnt_addr, 0, sizeof( clnt_addr ) ) ;
        clnt_addr_size = sizeof( clnt_addr ) ;

        // receive file_len
        int file_len = 0 ;
        if ( recvfrom( serv_sd, &file_len, sizeof( int ), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size ) == -1 ) {
            printf( "fail to read file len\n" ) ;
            exit( 1 ) ;
        }
        // printf("file len: %d\n", file_len) ;

        // receive filename
        char* filename = malloc( sizeof(char) * file_len ) ;
        if ( recvfrom( serv_sd, filename, file_len, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size ) == -1 ) {
            printf( "fail to read file name\n" ) ;
            exit( 1 ) ;
        }
        printf("filename: %s\n", filename) ;

        // open file
        FILE *fp ;
        if ( ( fp = fopen( filename, "wb" ) ) == NULL ) {
            printf( "fail to open file\n" ) ;
            exit( 1 ) ;
        }

        // receive filedata
        int read_cnt ;
        int pkt_cnt = 0 ;
        printf("start downloading\n\n") ;
        while( ( read_cnt = recvfrom( serv_sd, buffer, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size ) ) > 0 ) {
            // printf("%s", buffer) ;
            printf("%d ", read_cnt) ;
            fwrite( (void*)buffer, 1, read_cnt, fp ) ;
            pkt_cnt++ ;
            memset( buffer, 0, BUF_SIZE ) ;
            if ( read_cnt < BUF_SIZE ) {
                break ;
            }
        }
        printf( "\n%d packets arrived\n", pkt_cnt ) ;
        printf( "completed file transmit!\n--------------------------\n\n" ) ;
        fclose( fp ) ;
        free( filename ) ;
    }

    close( serv_sd );


    return 0 ;
}