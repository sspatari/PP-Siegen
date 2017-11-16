all: compute

compute: compute.cpp functions.cpp
	$(CXX) -pthread -g -o compute compute.cpp functions.cpp

clean:
	rm -f compute *~
