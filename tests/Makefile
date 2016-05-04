CXXFLAGS += -std=c++11 -ggdb
ifeq ($(COVER), YES)
    CXXFLAGS += -fprofile-arcs -ftest-coverage
endif
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=tests.o
FIDOLIB=../build/fido.a

ifneq ($(wildcard ../src/Simulator/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else
    $(info Compiling without SFML flags)
    LDFLAGS = -pthread
endif

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) $(FIDOLIB) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
