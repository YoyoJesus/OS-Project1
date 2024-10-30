CXX = g++
CXXFLAGS = -std=c++11 -pthread
TARGETS = producer consumer

#####################################################
# SET UP STUFF (DON'T MODIFY OR RUN THESE COMMANDS)
all: $(TARGETS)

producer: producer.o
	$(CXX) $(CXXFLAGS) -o producer producer.o -lrt

consumer: consumer.o
	$(CXX) $(CXXFLAGS) -o consumer consumer.o -lrt

producer.o: producer.cpp
	$(CXX) $(CXXFLAGS) -c producer.cpp

consumer.o: consumer.cpp
	$(CXX) $(CXXFLAGS) -c consumer.cpp

#####################################################

# Commands that can be run in this repository:
run: producer consumer
	./producer& ./consumer&

clean:
	rm -f *.o $(TARGETS)
	rm -f /dev/shm/shared_buffer
	pkill producer
	pkill consumer