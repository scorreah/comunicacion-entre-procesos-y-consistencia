// C program for the Server Side
 
// inet_addr
#include <arpa/inet.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>


using namespace std;

#define SOCKET_READ_TIMEOUT_SEC 60
 
// Semaphore variables
sem_t x, y;
pthread_t tid;
pthread_t writerthreads[100];
pthread_t readerthreads[100];
int readercount = 0;
 
// Vuelo 
int seats[36];

// Return available seats
string seatsState()
{
    string seats_str = "";
    for (int i = 0; i < 36; i++){
        if (seats[i] == 0)
            seats_str = seats_str + to_string(i) +',';
    }
	//string str = seats_str.c_str();
    return seats_str;
}

// Reader Function
void* reader(void* param)
{
	int newSocket = *((int*)param);
    // Lock the semaphore
    sem_wait(&x);
    readercount++;
 
    if (readercount == 1)
        sem_wait(&y);
    
    //Send avilable seats
    string buffer = seatsState();

    // Unlock the semaphore
    sem_post(&x);
    send(newSocket, buffer.c_str(), buffer.length(), 0);
 
    printf("\n%d Cliente está realizando una consulta\n", readercount);
 
    //sleep(5);
 
    // Lock the semaphore
    sem_wait(&x);
    readercount--;
    if (readercount == 0) {
        sem_post(&y);
    }
 
    // Lock the semaphore
    sem_post(&x);
 
    printf("\n%d Cliente salio de la consulta\n",
           readercount + 1);
    pthread_exit(NULL);
}
 
// Writer Function
void* writer(void* param)
{
    int newSocket = *((int*)param);
    printf("\nCliente intenta pedir reserva\n");
 
 
    printf("\nCliente esta reservando\n");
    // Receives the seat number
    int seat;
    recv(newSocket, &seat, sizeof(seat), 0);
    

    
    // Lock the semaphore
    sem_wait(&y);
    // Confirm seat available
    bool success;
    success = seats[seat] == 1 ? 0 : 1;
    string message;
    if (success) {
        // Send payment message
        string buffer1 = "Silla disponible (1 minuto para pagar) ";
        send(newSocket, buffer1.c_str(), buffer1.length(), 0);
        //Start 1 minute
        struct timeval timeout;
        timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC;
        timeout.tv_usec = 0;
        setsockopt(newSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        int pago;
        int recv_size = recv(newSocket, &pago, sizeof(pago), 0);
        if (recv_size == -1)
        {
            printf("Se acabo el tiempo, no pago.\n");
            message = "Lo siento, su asiento no pudo ser reservado";
        }else{
            if(pago == 1){
                printf("Se confirmo pago.\n");
                seats[seat]=1;
                message = "Su asiento ha sido reservado con exito";
            }else{
                message = "Se cancelo el pago";
            }
        }
    } else {
        message = "Lo siento, ese asiento ya se encuentra reservado";
    }
    
    // Unlock the semaphore
    sem_post(&y);
    printf("...\n");
	send(newSocket, message.c_str(), message.length(), 0);
    printf("\nCliente termino de reservar\n");
    pthread_exit(NULL);
}

// Writer Cancel Function
void* writerCancel(void* param)
{
    int newSocket = *((int*)param);
    printf("\nCliente esta tratando de cancelar\n");
 
    // Lock the semaphore
    sem_wait(&y);
 
    printf("\nCliente esta cancelando\n");
    int seat;
    recv(newSocket, &seat, sizeof(seat), 0);
    
    // Confirm seat available
    bool success;
    success = seats[seat] == 0 ? 0 : 1;
    seats[seat]=0;
    
    // Unlock the semaphore
    sem_post(&y);
    string message;
    if (success) {
        message = "Su asiento ha sido cancelado con exito";
    } else {
        message = "Lo siento, ese asiento no ha sido reservado aun";
    }
	send(newSocket, message.c_str(), message.length(), 0);
    printf("\nCliente sale de cancelación\n");
    pthread_exit(NULL);
}
 
// Driver Code
int main()
{
    // Initialize variables
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
 
    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
 
    // Bind the socket to the
    // address and port number.
    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));
 
    // Listen on the socket,
    // with 40 max connection
    // requests queued
    if (listen(serverSocket, 50) == 0)
        printf("Esperando conexión...\n");
    else
        printf("Error\n");
 
    // Array for thread
    pthread_t tid[60];
 
    int i = 0;
 
    while (1) {
        addr_size = sizeof(serverStorage);
 
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
        int choice = 0;
        recv(newSocket,
             &choice, sizeof(choice), 0);
 
        if (choice == 1) {
            // Creater readers thread
            if (pthread_create(&readerthreads[i++], NULL,
                               reader, &newSocket)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else if (choice == 2) {
            // Create writers thread
            if (pthread_create(&writerthreads[i++], NULL,
                               writer, &newSocket)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else if (choice == 3) {
            // Create writers thread
            if (pthread_create(&writerthreads[i++], NULL,
                               writerCancel, &newSocket)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else if (choice == 4) {
            string message;
            message = "Gracias por confiar en Apolo Airline, vuelva pronto!";
	        send(newSocket, message.c_str(), message.length(), 0);
        }

        //Cleaning the server requests
        if (i >= 50) {
            // Update i
            i = 0;
 
            while (i < 50) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(writerthreads[i++],
                             NULL);
                pthread_join(readerthreads[i++],
                             NULL);
            }
 
            // Update i
            i = 0;
        }
    }
 
    return 0;
}
