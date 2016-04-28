#include <vector>
#include "Polygon.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "grid.hpp"
#include <tuple>
#include "edge.hpp"
#include <algorithm>

using namespace Geometry;

// Constructor that takes as input the string filename

Grid::Grid(std::string filename): edges(), boundary(){ // Default constructor for edges and boundary
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
	/* collect all the edges in a vector of vectors of edges
	each component refers to a single polygon of the mesh */
	std::vector < std::vector <Edge > > edgevector(Nelems);
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
		// edge allocation
		Edge e1(v[0],v[1]),e2(v[1],v[2]),e3(v[2],v[0]);
		std::vector<Edge> E = {e1,e2,e3};
		edgevector[index] = E;
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
		// edge allocation
		Edge e1(v[0],v[1]),e2(v[1],v[2]),e3(v[2],v[3]),e4(v[3],v[0]);
		std::vector<Edge> E = {e1,e2,e3,e4};
		edgevector[index] = E;
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
		// edge allocation
		std::vector<Edge> E(vertix.size());
		for(std::size_t j=0; j<vertix.size()-1; j++){
			Edge e(vertix[j],vertix[j+1]);
			E[j] = e;
		}
		Edge e(vertix[vertix.size()-1],vertix[0]);
		E[vertix.size()-1] = e;
		edgevector[index] = E;
		} 
		}
	}
	// create the vector of all edges by progressive union
	edges = edgevector[0];
	for(std::size_t k=1; k < Nelems; k++){
		std::sort(edges.begin(),edges.end(),std::less<Edge>());
		std::sort(edgevector[k].begin(),edgevector[k].end(),std::less<Edge>());
		std::vector<Edge> temp;
		std::set_union(edges.begin(),edges.end(),edgevector[k].begin(),edgevector[k].end(),std::inserter(temp,temp.begin()),std::less<Edge>());
		edges = temp;
	}
	// boundary edges: set difference element by element
	boundary.resize(0); // initialization
	for(std::size_t k=0; k<Nelems; k++){
		std::vector<Edge> temp(edgevector[k]);
		for(std::size_t j = 0; j<k; j++){ // split the loop in two parts
		std::vector<Edge> temp0(0);
		std::sort(temp.begin(),temp.end(),std::less<Edge>());
		std::sort(edgevector[j].begin(),edgevector[j].end(),std::less<Edge>());
		std::set_difference(temp.begin(),temp.end(),edgevector[j].begin(),edgevector[j].end(),std::inserter(temp0,temp0.begin()),std::less<Edge>());
		temp = temp0;
		}
		for(std::size_t j = k+1; j<Nelems; j++){
		std::vector<Edge> temp0(0);
		std::sort(temp.begin(),temp.end(),std::less<Edge>());
		std::sort(edgevector[j].begin(),edgevector[j].end(),std::less<Edge>());
		std::set_difference(temp.begin(),temp.end(),edgevector[j].begin(),edgevector[j].end(),std::inserter(temp0,temp0.begin()),std::less<Edge>());
		temp = temp0;
		}
		for(std::size_t i=0; i<temp.size(); i++){
			boundary.emplace_back(temp[i]);
		}
	}
	std::sort(edges.begin(),edges.end(),std::less<Edge>());
	std::sort(boundary.begin(),boundary.end(),std::less<Edge>());
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

// Print the edges of the mesh

void Grid::printEdges(std::string which) const {
	if(which == "all")
	{
	std::cout << "Edges of the mesh:" << std::endl;
	std::cout << edges; // overloaded operator << for class Edge
	}
	else {
	if(which == "boundary")
	{
	std::cout << "Boundary edges:" << std::endl;
	std::cout << boundary; 
	}
	else {
	std::vector<Edge> internal;
	std::set_difference(edges.begin(),edges.end(),boundary.begin(),boundary.end(),
std::inserter(internal,internal.begin()),std::less<Edge>());
	std::cout << "Internal edges:" << std::endl;
	std::cout << internal; 
	}
	}
};


void Grid::printEdges(std::string which) { //non-const version
	if(which == "all")
	{
	std::cout << "Edges of the mesh:" << std::endl;
	std::cout << edges; // overloaded operator << for class Edge
	}
	else {
	if(which == "boundary")
	{
	std::cout << "Boundary edges:" << std::endl;
	std::cout << boundary; 
	}
	else {
	std::vector<Edge> internal;
	std::set_difference(edges.begin(),edges.end(),boundary.begin(),boundary.end(),
std::inserter(internal,internal.begin()),std::less<Edge>());
	std::cout << "Internal edges:" << std::endl;
	std::cout << internal; 
	}
	}
};
