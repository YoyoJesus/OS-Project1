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

void consumer(shared_data* data) {
    while (true) {
        sem_wait(&data->full); // wait for a full buffer
        sem_wait(&data->mutex); // wait for independent access to the buffer

        int item_consumed = 0;
        // consume the items in the buffer if there are any
        for (int i = 0; i < BUFFER_SIZE; i++) {
            item_consumed += data->buffer[i];
            data->buffer[i] = 0;
        }

        std::cout << "Consumer consumed: " << item_consumed << std::endl;

        sem_post(&data->mutex); // release the buffer
        sem_post(&data->empty); // signal that the buffer is empty and item has been consumed

        sleep(1);
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // seed the random number generator

    int shm_fd = shm_open("/sharedBuffer", O_RDWR, 0666);
    // if the shared memory failed to open
    if (shm_fd == -1) {
        std::cerr << "Error: Shared memory failed to open. C" << std::endl;
        return 1;
    }

    // map the shared memory
    shared_data* data = (shared_data*) mmap(0, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    // if the map failed
    if (data == MAP_FAILED) {
        std::cerr << "Error: Map failed." << std::endl;
        return 1;
    }

    std::thread consumer_thread(consumer, data);
    consumer_thread.join();

    return 0;
}