// C program for the Client Side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <string>
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
 

using namespace std;

// Function to send data to
// server socket.
void* clienthread(void* args)
{
 
    int client_request = *((int*)args);
    int network_socket;
 
    // Create a stream socket
    network_socket = socket(AF_INET,
                            SOCK_STREAM, 0);
 
    // Initialise port number and address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8989);
 
    // Initiate a socket connection
    int connection_status = connect(network_socket,
                                    (struct sockaddr*)&server_address,
                                    sizeof(server_address));
 
    // Check for connection error
    if (connection_status < 0) {
        puts("Error\n");
        return 0;
    }
 
    printf("Conexión establecida\n");
 
	int asientos[36];

    // Send data to the socket
    send(network_socket, &client_request,
         sizeof(client_request), 0);
	
    printf("\nCARGANDO ...\n");
	switch (client_request) {
    case 1: {
        char buffer[1024] = { 0 };
        recv(network_socket,
                &buffer, sizeof(buffer), 0);
        //displaySeats(buffer*);
        char * seats;
        seats = strtok (buffer, ",");
        if (seats != NULL){
            printf("\n-- SILLAS DISPONIBLES --\n");
            while(seats != NULL){
                if (atoi(seats) % 5 == 1 && atoi(seats) > 5) {
                    printf("\n%d ",atoi(seats)+1);
                } else {
                    printf("%d ",atoi(seats)+1);
                }
                seats = strtok(NULL,",");
            }
        }
        printf("\n-- // --\n");
		break;
    }
    case 2: {
		int seat;
		printf("\nIngrese el puesto que quiere seleccionar\n");
        printf("-> ");
    	scanf("%d", &seat);
        seat = seat-1;
		// Send data to the socket
    	send(network_socket, &seat, sizeof(seat), 0);
        //Receives payment option
        char buffer[1024] = { 0 };
        recv(network_socket, &buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
        // Payment 
        printf("Desea realizar el pago \n1. Si \n2. No\n");
        printf("-> ");
        int pago = 2;
        scanf("%d", &pago);
        send(network_socket, &pago, sizeof(pago), 0);
		break;
    }
    case 3: {
		int seat;
		printf("\nIngrese el puesto que quiere cancelar\n");
        printf("-> ");
    	scanf("%d", &seat);
        seat = seat-1;
		send(network_socket, &seat, sizeof(seat), 0);
		break;
    }
    case 4: {
		break;
    }
	default:
        printf("Entrada invalida\n");
        break;
    }

    // Receive data to the socket
    if (client_request != 1) {
        char buffer[1024] = { 0 };
        recv(network_socket,
                &buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
    }

    // Close the connection
    close(network_socket);
    pthread_exit(NULL);
 
    return 0;
}
 
// Driver Code
int main()
{
    printf("1. Solicitar información de asientos\n");
    printf("2. Hacer la reserva\n");
    printf("3. Cancelar una reserva\n");
    printf("4. Salir\n");
 
    // Input
    int choice;
    printf("-> ");
    scanf("%d", &choice);
    pthread_t tid;
 
    // Create connection
    // depending on the input
    switch (choice) {
    case 1: {
        int client_request = 1;
 
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 2: {
        int client_request = 2;
 
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 3: {
        int client_request = 3;
 
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    case 4: {
        int client_request = 4;
 
        // Create thread
        pthread_create(&tid, NULL,
                       clienthread,
                       &client_request);
        sleep(20);
        break;
    }
    default:
        printf("Invalid Input\n");
        break;
    }
 
    // Suspend execution of
    // calling thread
    pthread_join(tid, NULL);
}
