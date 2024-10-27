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

struct shared_data {
    sem_t mutex;
    sem_t full;
    sem_t empty;
    int buffer_table[2];
};

void consumer(shared_data *data) {
    while (true) {
        sem_wait(&data->full); // wait for an item to be placed in the buffer
        sem_wait(&data->mutex); // wait for independent access to the buffer

        int item = data->buffer_table[1];
        data->buffer_table[1] = 0;

        std::cout << "Consumer consumed: " << item << std::endl;

        sem_post(&data->mutex);
        sem_post(&data->empty);

        sleep(1);
    }
}

int main() {
    srand(time(0)); // seed the random number generator

    int shm_fd = shm_open("/sharedBuffer", O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error: Shared memory failed to open." << std::endl;
        return 1;
    }

    shared_data *data = (shared_data *) mmap(0, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "Error: Map failed." << std::endl;
        return 1;
    }

    std::thread consumer_thread(consumer, data);
    consumer_thread.join();

    return 0;
}