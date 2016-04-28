#include "Polygon.hpp"
#include "grid.hpp"
#include <iostream>
#include "GetPot.hpp"

//Use GetPot to parse the command line and take non default input files

void help(){
	std::cout << "Mesh allocation from input file" << std::endl;
	std::cout << "-i filename.dat to pass the input file, by default mesh.dat" << std::endl;
	std::cout << "Other possible input files: input.dat in.dat" << std::endl;
	std::cout << "-h or --help to print this help" << std::endl;
}

int main(int argc, char** argv){
	GetPot parse(argc,argv);
	if(parse.search(2,"-h","--help")){
		help();
		return 0;
	}
	std::string filename = parse.follow("mesh.dat","-i");
	// Define a Grid object
	const Grid grid(filename);
	std::cout << "Mesh area = " << grid.area() << std::endl;
	return 0;
}
		
	
	
