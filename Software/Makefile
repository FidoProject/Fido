CXXFLAGS += -std=c++11
ifeq ("$(wildcard $(FidoSim))", "")
    ifeq ("$(wildcard $(FidoSim))", "")
        $(info Compiling without SFML flags)
        LDFLAGS = -pthread
    endif
else
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
endif
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
