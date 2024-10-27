CXX = g++
CXXFLAG = -std=c++11 -pthread
TARGETS = consumer producer

all: $(TARGETS)

consumer: consumer.o
	$(CXX) $(CXXFLAG) -o consumer consumer.o -lrt

producer: producer.o
	$(CXX) $(CXXFLAG) -o producer producer.o -lrt

consumer.o: consumer.cpp
	$(CXX) $(CXXFLAG) -c consumer.cpp

producer.o: producer.cpp
	$(CXX) $(CXXFLAG) -c producer.cpp

run: $(TARGETS)
	./producer & ./consumer

clean:
	rm -f *.o $(TARGETS)
	rm -f /dev/shm/sem.* /dev/shm/shm.*
	pkill producer
	pkill consumer
