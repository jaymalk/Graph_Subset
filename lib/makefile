# Compiler to use
CXX = g++ -std=c++11

# Flags
CXXFLAGS = -Wall -g

# Executable name
OUT = main
OUT2 = main2

all: $(OUT) $(OUT2)

# Executable
$(OUT): main.o reader.o reduction.o graph.o
	$(CXX) reader.o graph.o reduction.o main.o -o main $(CXXFLAGS)

$(OUT2): main2.o
	$(CXX) main2.o -o main2 $(CXXFLAGS)

# Main
main.o: main.cpp graph.hpp reader.hpp reduction.hpp

# reader
reader.o: reader.hpp graph.hpp
	$(CXX) -c reader.hpp -o $@

# reduction
reduction.o: reduction.hpp graph.hpp
	$(CXX) -c reduction.hpp -o $@

# graph
graph.o: graph.hpp
	$(CXX) -c graph.hpp -o $@

# main2
main2.o: main2.cpp
	$(CXX) -c main2.cpp -o $@

# Clean
clean:
	rm *.o
	rm $(OUT)
	rm $(OUT2)