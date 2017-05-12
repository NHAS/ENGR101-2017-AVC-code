build : main.cpp
	g++ -o ADC_executable main.cpp -le101

clean : main.cpp
	rm ./main -rf
	rm ./main.o -rf
