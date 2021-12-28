#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#define BUF_SIZE 1024

typedef struct data {
    char data[BUF_SIZE] ;
} Data ;

typedef struct packet {
    int pkt_kind ; // 0: ack#, 1: pkt#, 2: FIN
    int pkt_num ; // 0 or 1, -1: not used
    int ack_num ; // 0 or 1, -1: not used
    int data_type ; // 0: file_name, 1: file_data, -1: some message
    int payload_len ;
    Data payload ; // data
} Packet ;

void
set_pkt( Packet** pkt, int pkt_kind, int pkt_num, int ack_num, int data_type, int payload_len, char* data ) {
    (*pkt)->pkt_kind = pkt_kind ;
    (*pkt)->pkt_num = pkt_num ;
    (*pkt)->ack_num = ack_num ;
    (*pkt)->data_type = data_type ;
    (*pkt)->payload_len = payload_len ;
    strcpy( (*pkt)->payload.data, data ) ;

    return ;
}


int 
main ( int argc, char *argv[] ) {
    if ( argc != 4 ) {
        printf("Usage: %s <IP> <PORT> <FILENAME>\n", argv[0]);
		exit(1);
    }

    // set socket
    int sd ;
    if ( ( sd = socket( PF_INET, SOCK_DGRAM, 0 ) ) == -1 ) {
        printf( "Fail to generate socket.\n" ) ;
        exit( 1 ) ;
    }
    // set timeout to 3 sec
    struct timeval optVal = { 2, 0 } ;
    int optLen = sizeof( optVal ) ;
    

    // set info
    struct sockaddr_in serv_addr ;
    memset( &serv_addr, 0, sizeof( serv_addr ) ) ;
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_addr.s_addr = inet_addr( argv[1] ) ;
    serv_addr.sin_port = htons( atoi( argv[ 2 ] ) ) ;

    setsockopt( sd, SOL_SOCKET, SO_RCVTIMEO, &optVal, optLen ) ;
    // info's server
    int serv_addr_size = sizeof( serv_addr ) ;


    int count = 0 ;
    char buffer[BUF_SIZE] ;
    memset( &buffer, 0, sizeof( BUF_SIZE ) ) ;
    int total = 0 ;
    int read_cnt ;
    int pkt_cnt = 0 ;
    int pkt_num = 0 ;
    int retrans = 0 ;
    struct timeval start, end ;
    gettimeofday(&start, NULL) ;
    FILE* fp = NULL ;
    // pkt_send
    Packet* pkt_send = malloc( sizeof( Packet ) ) ;
    memset( pkt_send, 0, sizeof( Packet ) ) ;
    // pkt_recv
    Packet* pkt_recv = malloc( sizeof( Packet ) ) ;
    memset( pkt_recv, 0, sizeof( Packet ) ) ;

    // send a file data to server
    while ( 1 ) {
        // when file is not opend
        if ( fp == NULL ) {
            // open file
            if ( ( fp = fopen( argv[3], "rb" ) ) == NULL ) {
                printf( "Fail to open %s.\n", argv[3] ) ;
                exit( 1 ) ;
            }
            // set pkt option
            set_pkt( &pkt_send, 1, pkt_num, -1, 0, strlen(argv[3]) ,argv[3] ) ;

            // send pkt
            if ( ( read_cnt = sendto( sd, pkt_send, sizeof( Packet ), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ) == -1 ) {
                printf( "Fail to send packet.\n" ) ;
                exit( 1 ) ;
            }

            printf( "Send pkt%d to server.\n", pkt_send->pkt_num ) ;
            pkt_cnt++ ;
            total += read_cnt ;
        }
        // when file is opend
        else {
            // end of file
            if ( feof( fp ) ) {
                // send pkt option to FIN
                set_pkt( &pkt_send, 2, pkt_num, -1, -1, 0,"nothing in payload" ) ;
                
                // send pkt
                if ( (read_cnt = sendto( sd, pkt_send, sizeof( Packet ), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ) == -1 ) {
                    printf( "Fail to send packet.\n" ) ;
                    exit( 1 ) ;
                }
                printf( "Send FIN with pkt%d to server.\n", pkt_num ) ;
                pkt_cnt++ ;
                total += read_cnt ;
            }
            else {
                printf( "Send pkt%d to server.\n", pkt_num ) ;
                count = fread( (void*) buffer, 1, BUF_SIZE - 1, fp ) ;
                // printf("afadsfsdfafsdfds%s\n", buffer) ;
                buffer[count] = '\0' ;
                // set pkt option
                set_pkt( &pkt_send, 1, pkt_num, -1, 1, count, buffer ) ;

                // send pkt
                if ( (read_cnt = sendto( sd, pkt_send, sizeof( Packet ), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ) == -1 ) {
                    printf( "Fail to send packet.\n" ) ;
                    exit( 1 ) ;
                }

                pkt_cnt++ ;
                total += read_cnt ;
            }
        }

        // -----------------------
        // receive ack from server, until receive correct ACK
        while ( 1 ) {
            if ( recvfrom( sd, pkt_recv, sizeof( Packet ), 0, (struct sockaddr*)&serv_addr, &serv_addr_size ) == -1 ) {
                printf( "Retransmisstion.(timeout)\n" ) ;
                retrans++ ;
                // pkt_send->pkt_kind = 3 ;
                if ( ( read_cnt = sendto( sd, pkt_send, sizeof( Packet ), 0, (struct sockaddr*) &serv_addr, sizeof( serv_addr ) ) ) == -1 ) {
                    printf( "Fail to send packet.\n" ) ;
                    exit( 1 ) ;
                }

                pkt_cnt++ ;
                total += read_cnt ;
            } 
            // not timeout
            else {
                printf( "Receive ack%d from server.\n", pkt_recv->ack_num ) ;
                printf( "(Retransmission: %d)\n", retrans ) ;
                // case: no packet loss
                if ( pkt_recv->ack_num == pkt_num && pkt_recv->pkt_kind == 0 ) {
                    pkt_num = 1 - pkt_num ;
                    break ;
                }
                // case: ignore packet
                else if ( pkt_recv->ack_num != pkt_num && pkt_recv->pkt_kind == 0 ){
                    // printf( "------receive from server ack#: ack%d\n", pkt_recv->ack_num ) ;
                    printf( "Ignore packet.\n" ) ;
                    continue ;
                }
                // case: finish
                else if ( pkt_recv->pkt_kind == 2 ) {
                    break ;
                }
            }
        }
        printf( "-------------------\n" ) ;
        // case: finish
        if ( pkt_recv->pkt_kind == 2 )
            break ;

        // buffer 초기화
        memset( &buffer, 0, BUF_SIZE ) ;
        memset( pkt_send, 0, sizeof( Packet ) ) ;
        memset( pkt_recv, 0, sizeof( Packet ) ) ;
    }
    
    gettimeofday(&end, NULL) ;
    double time = (double)end.tv_sec - (double)start.tv_sec + ((double)end.tv_usec - (double)start.tv_usec) / 1000000 ;

    printf( "\n- Success to send file.\n" ) ;
    printf( "- Packet size: %d.\n", sizeof(Packet) ) ;
    printf( "- Sending packet: %d.\n", pkt_cnt ) ;
    printf( "- Total bytes: %d bytes.\n", total ) ;
    printf( "- Elapsed time: %lf sec.\n", time ) ;
    printf( "- Throughput: %lf byte/sec.\n", (double)total / time) ;
    printf( "- Retransmission: %d\n", retrans ) ;

    fclose( fp );
    close( sd ) ;
    free( pkt_send ) ;
    free( pkt_recv ) ;

    return 0 ;
}

