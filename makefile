build : main.cpp
	g++ -o AVC_program main.cpp -le101 -std=c++11 

clean : main.cpp
	rm ./AVC_program -rf
	rm ./main.o -rf
	
