all: main.exe

main.exe : 
	@g++ -Wall -Wextra -std=c++17 main.cpp -o main.exe

test :
	@make main.exe
	./main.exe
	@make clean

clean : 
	@rm main.exe
