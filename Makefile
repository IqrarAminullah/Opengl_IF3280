default:
	g++ -o main tutorial08.cpp glad.c common/*.cpp -ldl -lGL -lglfw3 -lGLEW -lX11 -pthread
