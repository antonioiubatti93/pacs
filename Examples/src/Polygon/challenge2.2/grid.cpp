#include <vector>
#include "Polygon.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "grid.hpp"
#include <tuple>
#include <algorithm>

using namespace Geometry;

// Constructor that takes as input the string filename

Grid::Grid(std::string filename): A(0.) { // Default constructor for edges and boundary
	//reading line by line from a generic input file ifile
	std::ifstream ifile;
	ifile.open(filename,std::ios::in);
	/*Assume that the first line of the file contains two only numbers,
	which are the number of vertices and that of polygons*/
	unsigned int Npoints,Nelems;
	//read the first line separately
	std::string aux;
	std::getline(ifile,aux);
	std::istringstream iss(aux);
	iss >> Npoints >> Nelems;
	//resize the class members
	points.resize(Npoints);
	elems.resize(Nelems);
	for(std::size_t k=0; k<Npoints; k++) //initialization of points
	{
		std::size_t index;
		double xx,yy;
		std::getline(ifile,aux);
		std::istringstream is(aux);
		is >> index >> xx >> yy;
		points[index].set(xx,yy);
	}
	for(std::size_t k=0; k<Nelems; k++)
	{
		std::getline(ifile,aux);
		std::istringstream is(aux);
		std::size_t index;
		unsigned int type;
		is >> index >> type;
		//set the shape of the polygon
		if(type == 0)
		{
		std::vector<std::size_t> v(3);
		is >> v[0] >> v[1] >> v[2];
		Triangle triangle(v,points); // new constructor: only vertices indexes and points vector
		elems[index] = std::make_shared<Triangle>(triangle);
		A += elems[index]->area();
		}
		else 
		{ 
		if(type == 1)
		{
		std::vector<std::size_t> v(4);
		is >> v[0] >> v[1] >> v[2] >> v[3];
		Square square(v,points);
		elems[index] = std::make_shared<Square>(square);
		A += elems[index]->area();
		}
		else{ // Any other character is interpreted as 'other polygon' for simplicity
		std::vector<std::size_t> v;
		std::size_t c;
		while(is >> c){
			v.emplace_back(c);
		}
		Polygon polygon(v,points);
		elems[index] = std::make_shared<Polygon>(polygon);
		A += elems[index]->area();
		} 
		}
	}
	ifile.close();
};


// Return the area of the mesh

double Grid::area() const {
	return A;
};

double Grid::area() {
	return A;
};
