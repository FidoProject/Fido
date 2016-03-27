CXXFLAGS += -std=c++11
ifneq ($(wildcard src/FidoSim/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else ifneq ($(wildcard src/FidoKin/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
else
    $(info Compiling without SFML flags)
    LDFLAGS = -pthread
endif

$(info $(LDFLAGS))

SOURCES=$(shell find src/ -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
BINS=$(OBJECTS:src/%=bin/%)
TARGET=src/foo.o
LIB=build/fido-lib.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@ $(LDFLAGS)
	@mkdir -p bin/
	@mv src/*.o bin/
	@echo "Made object files in ../bin/"

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJECTS) $(BINS) $(LIB)
	@echo "Deleted files"
	@rm -rf bin/ build/
	@echo "Deleted folders"

.PHONY: lib
lib: all
	@mkdir -p build/
	@ar rvs $(LIB) $(BINS)
	@echo "Built library at $(LIB)"
