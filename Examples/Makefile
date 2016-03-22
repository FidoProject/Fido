CXXFLAGS += -std=c++11 -pthread ../Software/fido-lib.a
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%)

.PHONY: all
all: $(OBJECTS)

.PHONY: clean
clean:
	rm -f $(OBJECTS)
