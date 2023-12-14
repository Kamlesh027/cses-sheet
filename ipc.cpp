#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

// Define the key for the shared memory segment
#define SHM_KEY 1234

int main() {
    int shmid;
    char *shared_memory;

    // Create a shared memory segment
    shmid = shmget(SHM_KEY, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment to the process's address space
    shared_memory = (char*)shmat(shmid, NULL, 0);
    if (shared_memory == (char*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Write data to the shared memory
    cout << "Enter data to be written to shared memory: ";
    cin.getline(shared_memory, 1024);

    // Detach the shared memory segment
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // The producer process exits

    // Now, let's create a consumer process to read from the shared memory

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Consumer Process

        // Attach the shared memory segment to the process's address space
        shared_memory = (char*)shmat(shmid, NULL, 0);
        if (shared_memory == (char*)-1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }

        // Read data from the shared memory
        cout << "Consumer Process: Data read from shared memory: " << shared_memory << endl;

        // Detach the shared memory segment
        if (shmdt(shared_memory) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }

        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process (producer) waits for the child process (consumer) to finish
        wait(NULL);
    }

    return 0;
}
