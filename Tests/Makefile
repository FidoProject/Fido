CXXFLAGS= -std=c++11 -pthread
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=test
FIDOLIB=../Software/fido-lib.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(FIDOLIB) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
