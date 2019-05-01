default:
	g++ -o main main.cpp glad.c common/*.cpp -ldl -lGL -lglfw3 -lGLEW -lX11 -pthread -std=c++11
