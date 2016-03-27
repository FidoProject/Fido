CXXFLAGS += -std=c++11
SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=tests.o
FIDOLIB=../build/fido-lib.a

ifneq ($(wildcard ../Software/FidoSim/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else ifneq ($(wildcard ../Software/FidoKin/.*),)
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
