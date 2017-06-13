//PARALLEL VERSION
#include <iostream>
#include <vector>
#include "pp/Pipe.hpp"

std::vector<int> in(10);

int FuncMap(int in){
	return in * 3;
};

bool FuncFind(int in){
	if(in % 2 == 0){
		return true;
	}

	return false;
};

typedef int (*mapF)(int);
typedef bool (*findF)(int);

int main(int argc, char *argv[]) {

	for(unsigned int i = 0; i < in.size(); i++){
		in[i] = i;
	};

	pp::Pipe pipe;
	int sum = pipe.add_to_pipe<int>(in.begin(), in.end())
			//.parallel() - dosn't work
			.map< int, int, mapF>(FuncMap)
			//.find< int, findF >(([&](int in){if(in % 2 == 0) return true; return false;}))
			.find< int, findF >(FuncFind)
			.sum< int >();

	std::cout<<"Total: "<<sum<<"\n";

	return 0;
}


