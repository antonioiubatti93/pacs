#include <vector>
#include "Polygon.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "grid.hpp"
#include <tuple>

using namespace Geometry;

// Constructor that takes as input the string filename

Grid::Grid(std::string filename){
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
	for(std::size_t k=0; k<Npoints; k++) 
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
		std::array<std::size_t,3> v;
		is >> v[0] >> v[1] >> v[2];
		const std::vector<Point2D> vertices = {points[v[0]],points[v[1]],points[v[2]]};
		Triangle triangle(vertices);
		elems[index] = std::make_shared<Triangle>(triangle);
		}
		else 
		{ 
		if(type == 1)
		{
		std::array<std::size_t,4> v;
		is >> v[0] >> v[1] >> v[2] >> v[3];
		std::vector<Point2D> vertices = {points[v[0]],points[v[1]],points[v[2]],points[v[3]]};
		Square square(vertices);
		elems[index] = std::make_shared<Square>(square);
		}
		else{ // Any other character is interpreted as 'other polygon' for simplicity
		std::vector<std::size_t> vertix;
		std::size_t c;
		while(is >> c){
			vertix.emplace_back(c);
		}
		std::vector<Point2D> vertices(vertix.size());
		for(std::size_t j=0; j<vertix.size(); j++)
			vertices[j]=points[vertix[j]];
		Polygon polygon(vertices);
		elems[index] = std::make_shared<Polygon>(polygon);
		} 
		}
	}
	
	ifile.close();
};


// Compute the area of the mesh

double Grid::area() const {
	double A{0.};
	for(std::size_t k = 0; k < elems.size(); k++)
		A+=elems[k]->area();
	return A;
};

double Grid::area() {
	double A{0.};
	for(std::size_t k = 0; k < elems.size(); k++)
		A+=elems[k]->area();
	return A;
};
