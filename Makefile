main: src/main.cpp
	g++ -std=c++20 -Iinclude src/main.cpp src/cpu.cpp src/utils.cpp src/system.cpp src/gpu.cpp src/ram.cpp -o lhmon \
  	-lftxui-component -lftxui-dom -lftxui-screen

clean:
	rm lhwmon
