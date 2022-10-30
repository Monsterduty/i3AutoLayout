CC = g++

i3-autolayout: main.cpp

	$(CC) main.cpp -o i3-autolayout -lX11
