CXX = g++
CXXFLAGS = -std=c++20 -lgdal

SOURCES := $(wildcard *.cc)
EXECUTABLE = ChartPlotter

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
