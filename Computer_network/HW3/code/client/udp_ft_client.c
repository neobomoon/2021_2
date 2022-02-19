#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define BUF_SIZE 50

int 
main ( int argc, char *argv[] ) {
    if ( argc != 4 ) {
        printf("Usage: %s <IP> <PORT> <FILENAME>\n", argv[0]);
		exit(1);
    }

    // open sending file
    FILE* fp ;
    if ( ( fp = fopen( argv[3], "rb" ) ) == NULL ) {
        printf( "fail to open %s\n", argv[3] ) ;
        exit( 1 ) ;
    }

    // set socket
    int sd ;
    if ( ( sd = socket( PF_INET, SOCK_DGRAM, 0 ) ) == -1 ) {
        printf( "fail to generate socket\n" ) ;
        exit( 1 ) ;
    }
    // set info for tcp trans
    struct sockaddr_in serv_addr ;
    memset( &serv_addr, 0, sizeof( serv_addr ) ) ;
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_addr.s_addr = inet_addr( argv[1] ) ;
    serv_addr.sin_port = htons( atoi( argv[ 2 ] ) ) ;


    // send a file name to server
    printf( "len of filename %d\n", strlen( argv[ 3 ] ) * sizeof( char ) ) ;
    int file_len = strlen( argv[3] ) ;
    // send a file_len
    sendto( sd, &file_len, sizeof(int), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr) ) ;
    
	// send a filename
	sendto( sd, argv[3], strlen( argv[ 3 ] ), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ;

    // send a file data to server
    int count = 0 ;
    char buffer[BUF_SIZE] = {0, } ;
    int total = 0 ;
    int read_cnt ;
    int pkt_cnt = 0 ;
    struct timeval start, end ;
    gettimeofday(&start, NULL) ;
    while ( feof( fp ) == 0 ) {
        count = fread( (void*) buffer, 1, BUF_SIZE, fp ) ;
        // printf("%s", buffer) ;

        if ( count < BUF_SIZE ) {
            read_cnt = sendto( sd, buffer, count, 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ;
            printf("%d ", read_cnt) ;
            pkt_cnt++ ;
            total += count ;
            break ;
        }
        read_cnt = sendto( sd, buffer, count, 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ;
        printf("%d ", read_cnt) ;
        pkt_cnt++ ;
        memset( buffer, 0, BUF_SIZE ) ;
        total += count ;
    }

    gettimeofday(&end, NULL) ;
    double time = (double)end.tv_sec - (double)start.tv_sec + ((double)end.tv_usec - (double)start.tv_usec) / 1000000 ;

    printf( "\nsuccess to send file\n" ) ;
    printf( "\n%d packets arrived\n", pkt_cnt ) ;
    printf( "total bytes: %d bytes\n", total ) ;
    printf( "elapsed time: %lf sec\n", time ) ;
    printf( "throughput: %lf byte/sec\n", (double)total / time) ;
    fclose( fp );
    close( sd ) ;

    return 0 ;
}

