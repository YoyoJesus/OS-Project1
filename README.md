# OS-Project1 - The Consumer Producer Problem
- *Tested on CentOS via Kent State University's Wasp/Hornet Servers*

## Project Assignment:

In this programming assignment, you must provide the source codes, documents, and examples in detail on Github. They will be counted for the final score. Please submit the link to the project on Github. 

### Requirements:
- You are required to use C/C++ in Linux/Unix. You can consider installing VirtualBox in your non-Linux environment.
- Programs must succeed in compilation and execution as required (80 points)
- Readme, documentation and example are required (20 points).

### Topic: Producer-Consumer Problem

The producer generates items and puts items onto the table. The consumer will pick up items. The table can only hold two items at the same time. When the table is complete, the producer will wait. When there are no items, the consumer will wait. We use semaphores to synchronize producer and consumer.  Mutual exclusion should be considered. We use threads in the producer program and consumer program. Shared memory is used for the “table”.

## Running this Code:

After cloning the repository locally, this program can be compiled and executed using the following command:
```{bash}
make run
```

To stop execution run the command:
```{bash}
make clean
```

The program will run continuously in the background across two threads until the above command is run.

## Alternative way to compile & run

You can also run the code manually using the following commands:
```{bash}
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer
./producer& ./consumer&
```

To stop execution, run the following:
```{bash}
pkill consumer & pkill producer
```

Then to clean up the create files, run these commands:
```{bash}
rm -f *.o producer consumer
rm -f /dev/shm/sharedBuffer
```

## Example Execution:
Here is what the code will look like when it is running properly:
```{bash}
[asternb1@wasp OS-Project1]$ make run
g++ -std=c++11 -pthread -c producer.cpp
g++ -std=c++11 -pthread -o producer producer.o -lrt
g++ -std=c++11 -pthread -c consumer.cpp
g++ -std=c++11 -pthread -o consumer consumer.o -lrt
./producer& ./consumer&
Producer produced: 88
Consumer consumed: 88
Producer produced: 144
Consumer consumed: 144
Producer produced: 166
Consumer consumed: 166
Producer produced: 62
Consumer consumed: 62
Producer produced: 116
Consumer consumed: 116
```