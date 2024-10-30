// Austin Sternberg
// Operating Systems
// Coding Program 1
// 10/27/2024

#include <iostream>
#include <semaphore.h>
#include <thread>
#include <ctime>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>

constexpr int BUFFER_SIZE = 2;

struct shared_data {
    sem_t empty;
    sem_t full;
    sem_t mutex;
    int buffer[BUFFER_SIZE];
};

void producer(shared_data* data) {
    while (true) {
        sem_wait(&data->empty); // wait for an empty buffer
        sem_wait(&data->mutex); // wait for independent access to the buffer

        int item_produced = 0;
        // produce the items in the buffer if there are any
        for (int i = 0; i < BUFFER_SIZE; i++) {
            data->buffer[i] = rand() % 100 + 1;
            item_produced += data->buffer[i];
        }

        std::cout << "Producer produced: " << item_produced << std::endl;

        sem_post(&data->mutex); // release the buffer
        sem_post(&data->full); // signal that the buffer is full and item has been produced

        sleep(1);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // seed the random number generator

    //create shared memory
    int shm_fd = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);
    // if the shared memory failed to open
    if (shm_fd == -1) {
        std::cerr << "Error: Shared memory failed to open. P" << std::endl;
        return 1;
    }

    // set the size of the shared memory
    ftruncate(shm_fd, sizeof(shared_data));
    //check that shared memory was created
    if (shm_fd == -1) {
        std::cerr << "Error: Shared memory failed to create. P" << std::endl;
        return 1;
    }

    // map the shared memory
    shared_data* data = (shared_data*) mmap(0, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    // if the map failed
    if (data == MAP_FAILED) {
        std::cerr << "Error: Map failed." << std::endl;
        return 1;
    }

    // initialize the semaphores
    sem_init(&data->empty, 1, BUFFER_SIZE); // buffer is empty
    sem_init(&data->full, 1, 0); // nothing in the buffer at the start
    sem_init(&data->mutex, 1, 1); // mutex for the buffer - ensure mutual exclusion

    std::thread producer_thread(producer, data);
    producer_thread.join();

    return 0;
}