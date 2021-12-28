#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
    if ( argc != 2 ) {
		printf( "Usage: %s <port>\n", argv[0] );
		exit( 1 );
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
        printf( "bind() error" ) ;
        exit( 1 ) ;
    }

    // file transmit
    struct sockaddr_in clnt_addr ;
    int clnt_addr_size = sizeof( clnt_addr ) ;
    memset( &clnt_addr, 0, sizeof( clnt_addr ) ) ;

    char buffer[BUF_SIZE] ;
    memset( &buffer, 0, sizeof( BUF_SIZE ) ) ;

    FILE *fp ;
    while( 1 ) {
        int ack_num = 0 ;
        int pkt_cnt = 0 ;
        while( 1 ) {
            // pkt_send
            Packet* pkt_send = malloc( sizeof( Packet ) ) ;
            memset( pkt_send, 0, sizeof( Packet ) ) ;

            // pkt_recv
            Packet* pkt_recv = malloc( sizeof( Packet ) ) ;
            memset( pkt_recv, 0, sizeof( Packet ) ) ;

            // receive packet
            if ( recvfrom( serv_sd, pkt_recv, sizeof( Packet ), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size ) == -1 ) {
                printf( "Fail to receive packet.\n" ) ;
                fclose( fp ) ;
                free( pkt_send ) ;
                free( pkt_recv ) ;
                exit(1) ;
            }
            printf( "Receive pkt%d from client.\n", pkt_recv->pkt_num ) ;

            pkt_cnt++ ;

            if ( pkt_recv->data_type == 0 && pkt_recv->pkt_kind == 1 && pkt_recv->pkt_num == ack_num ) {
                // open file & change ack_num
                printf("File name: %s\n", pkt_recv->payload.data) ;
                if ( ( fp = fopen( pkt_recv->payload.data, "wb" ) ) == NULL ) {
                    printf( "Fail to open file.\n" ) ;
                    fclose( fp ) ;
                    free( pkt_send ) ;
                    free( pkt_recv ) ;
                    exit( 1 ) ;
                }
                ack_num = 1 - ack_num ;
                set_pkt( &pkt_send, 0, -1, pkt_recv->pkt_num, -1, 0, "ACK SIGNAL about filename" ) ;
            }
            // if data_type = 1 (data)
            else if ( pkt_recv->data_type == 1 && pkt_recv->pkt_kind == 1 && pkt_recv->pkt_num == ack_num ) {
                // write to the file & change ack_num
                printf("%d\n", pkt_recv->payload_len ) ;
                fwrite( pkt_recv->payload.data, 1, pkt_recv->payload_len, fp ) ;
                // printf("%s\n", pkt_recv->payload.data) ;
                ack_num = 1 - ack_num ;
                set_pkt( &pkt_send, 0, -1, pkt_recv->pkt_num, -1, 0, "ACK SIGNAL about file data" ) ;
            }
            // if pkt_kind = 2 (FIN)
            else if ( pkt_recv->pkt_kind == 2 && pkt_recv->pkt_num == ack_num ) {
                // close file & change ack_num 
                printf( "(+Receive FIN from client)\n" ) ;
                ack_num = 1 - ack_num ;
                set_pkt( &pkt_send, 2, -1, pkt_recv->pkt_num, -1, 0, "FIN SIGNAL about client side FIN SIGNAL" ) ;
            }
            // when receive dup packet
            else {
                printf("dup ack\n") ;
                set_pkt( &pkt_send, 0, -1, pkt_recv->pkt_num, -1, 0, "It was dup packet" ) ;
            }

            // -----------------------
            // send response packet to cilent
            // send ack# to client
            if ( sendto( serv_sd, pkt_send, sizeof( Packet ), 0, (struct sockaddr*) &clnt_addr, sizeof( clnt_addr ) ) == -1 ) {
                printf( "Fail to send ACK SIGNAL.\n" ) ;
                fclose( fp ) ;
                free( pkt_send ) ;
                free( pkt_recv ) ;
                exit( 1 ) ;
            }
            
            // FIN
            if ( pkt_send->pkt_kind == 2 ) {
                printf( "Send ack%d to client.\n", pkt_send->ack_num ) ;
                printf( "-------------------\n" ) ;
                break ;
            }
            printf( "Send ack%d to client.\n", pkt_send->ack_num ) ;
            printf( "-------------------\n" ) ;

            free( pkt_send ) ;
            free( pkt_recv ) ;
        }


        printf( "\n%d Packets arrived.\n", pkt_cnt ) ;
        printf( "Completed file transmit!\n--------------------------\n\n" ) ;
        fclose( fp ) ;
    }

    close( serv_sd );


    return 0 ;
}