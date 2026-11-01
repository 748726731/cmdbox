# CmdBox —— 终端里的小抄本
# 用法：make        编译出 ./cmdbox
#       make run    编译并直接运行
#       make clean  删掉二进制

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -O2

# main.cpp 一个文件把所有 .h/.cpp 都 include 进来，
# 所以这些 -I 一个都不能少。
INCS := -Idatabase/include -Idatabase/src \
        -Ibackend/include  -Ibackend/src \
        -Ifrontend/include -Ifrontend/src

cmdbox: main.cpp $(wildcard */*/*.h */*/*.cpp)
	$(CXX) $(CXXFLAGS) $(INCS) main.cpp -o $@

run: cmdbox
	./cmdbox

clean:
	rm -f cmdbox

install: cmdbox
	mkdir -p $(HOME)/.local/bin
	cp cmdbox $(HOME)/.local/bin/cmdbox
	@echo "已装到 $(HOME)/.local/bin/cmdbox"
	@echo "注意：data.txt 是跟着二进制走的，第一次运行会出现在 $(HOME)/.local/bin/data.txt"

.PHONY: run clean install
