#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MSG_SIZE 100  // Maximum message size

// Caesar cipher encryption function
void caesar_cipher(char* message, int key) {
    for (int i = 0; message[i] != '\0'; i++) {
        if (isalpha(message[i])) {
            char base = isupper(message[i]) ? 'A' : 'a';
            message[i] = (message[i] - base + key) % 26 + base;
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);  
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {  
        char message[MSG_SIZE] = "Hello World!";
        int key = 3;  // Caesar cipher shift key
        int length = strlen(message);

        printf("Master: Sending message \"%s\" with key %d\n", message, key);
        MPI_Send(&length, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&key, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(message, length + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(message, MSG_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Master: Received encrypted message: \"%s\"\n", message);

    } else if (rank == 1) {  
        char message[MSG_SIZE];
        int key, length;

        MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&key, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(message, MSG_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Worker: Received message \"%s\" with key %d, encrypting...\n", message, key);
        caesar_cipher(message, key);

        MPI_Send(message, length + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
