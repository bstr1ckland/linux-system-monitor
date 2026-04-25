main: src/main.cpp
	g++ -Iinclude src/main.cpp src/cpu.cpp src/utils.cpp src/system.cpp -o main \
  	-lftxui-component -lftxui-dom -lftxui-screen

clean:
	rm main