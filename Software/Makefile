CXXFLAGS += -std=c++11
ifneq ($(wildcard ./FidoSim/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else ifneq ($(wildcard ./FidoKin/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else
    $(info Compiling without SFML flags)
    LDFLAGS = -pthread
endif

$(info $(LDFLAGS))

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
lib: all
	ar rvs fido-lib.a $(OBJECTS)
