CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = mapreduce_demo
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)
