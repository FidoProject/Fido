CXXFLAGS += -std=c++11
ifeq ($(COVER), YES)
    CXXFLAGS += -fprofile-arcs -ftest-coverage
endif
PREFIX?=/usr/local
ifneq ($(wildcard src/Simulator/.*),)
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
TARGET=bin/foo.o
LIB=build/fido.a

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin/
	@cp src/*.o bin/
ifneq ($(LDFLAGS), -pthread)
	    @mkdir -p bin/Simulator/
	    @cp src/Simulator/*.o bin/Simulator/
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
	cp -r include/* $(DESTDIR)/$(PREFIX)/include/Fido/
