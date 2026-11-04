CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -O2
INCS := -Idatabase/inlcude -Idatabase/src -Ibackend/include -Ibackend/src -Ifrontend/include -Ifrontend/src

cmdbox: main.cpp $(wildcard database/*/*.h database/*/*.cpp backend/*/*.h backend/*/*.cpp frontend/*/*.h frontend/*/*.cpp)
	$(CXX) $(CXXFLAGS) $(INCS) main.cpp -o cmdbox

run: cmdbox
	./cmdbox

clean:
	rm -f cmdbox

install: cmdbox
	mkdir -p $(HOME)/.local/bin
	cp cmdbox $(HOME)/.local/bin/cmdbox

.PHONY: run clean install
