CXXFLAGS += -std=c++11
PREFIX?=/usr/local
ifneq ($(wildcard src/FidoSim/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    SFML = "YES"
else ifneq ($(wildcard src/FidoKin/.*),)
    $(info Compiling with SFML flags)
    LDFLAGS = -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    SFML = "YES"
else
    $(info Compiling without SFML flags)
    LDFLAGS = -pthread
endif

$(info $(LDFLAGS))

SOURCES=$(shell find src/ -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
BINS=$(OBJECTS:src/%=bin/%)
TARGET=bin/foo.o
LIB=build/fido-lib.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin/
	@cp src/*.o bin/
ifneq ($(SFML), )
	    @mkdir -p bin/FidoKin/
	    @mkdir -p bin/FidoSim/
	    @cp src/FidoKin/*.o bin/FidoKin/
	    @cp src/FidoSim/*.o bin/FidoSim/
endif
	$(LINK.cpp) $(BINS) $(LOADLIBES) $(LDLIBS) -o $@ $(LDFLAGS)
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

.PHONY: install
install: lib
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(LIB) $(DESTDIR)/$(PREFIX)/lib/
	mkdir -p $(DESTDIR)/$(PREFIX)/include/Fido/
	cp include/* $(DESTDIR)/$(PREFIX)/include/Fido/
