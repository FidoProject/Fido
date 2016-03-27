CXXFLAGS += -std=c++11
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
LIB=fido.a
LIBPATH=build/

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
	@echo "Made object files in bin/"

.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJECTS) $(BINS) $(LIBPATH)$(LIB)
	@echo "Deleted files"
	@rm -rf bin/ build/
	@echo "Deleted folders"

.PHONY: lib
lib: all
	@mkdir -p $(LIBPATH)
	@ar rvs $(LIBPATH)$(LIB) $(BINS)
	@echo "Built library at $(LIBPATH)"

.PHONY: install
install: lib
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(LIBPATH)$(LIB) $(DESTDIR)/$(PREFIX)/lib/
	mkdir -p $(DESTDIR)/$(PREFIX)/include/Fido/
	cp -r include/ $(DESTDIR)/$(PREFIX)/include/Fido/

.PHONY: uninstall
uninstall: clean
	@rm $(DESTDIR)/$(PREFIX)/lib/$(LIB)
	@echo "Removed library"
	@rm -rf $(DESTDIR)/$(PREFIX)/include/Fido
	@echo "Removed includes"
