build : main.cpp
	g++ -o main main.cpp -le101

clean : main.cpp
	rm ./main -rf
	rm ./main.o -rf
