main: src/main.cpp
	g++ -Iinclude src/main.cpp src/cpu.cpp -o main

clean:
	rm main