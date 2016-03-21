CXXFLAGS += -std=c++11 -pthread
LDFLAGS = -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=foo.o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: lib
lib:
	ar rvs fido-lib.a $(OBJECTS)
