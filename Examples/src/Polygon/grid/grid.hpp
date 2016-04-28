#ifndef GRID_CLASS_HPP
#define GRID_CLASS_HPP
#include <vector>
#include "Polygon.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include "edge.hpp"
#include <string>

using namespace Geometry;

class Grid{
	public:
	Grid() = default;
	Grid & operator=(Grid const & grid) = default;
	~Grid() = default;
	Grid(std::string filename);
	double area() const;
	double area();
	void printEdges(std::string which) const;
	void printEdges(std::string which);
	friend std::ostream & operator <<(std::ostream &, std::vector<Edge> const &);
	private:
	std::vector<Point2D> points;
	std::vector<std::shared_ptr<AbstractPolygon > > elems;
	std::vector<Edge> edges;
	std::vector<Edge> boundary;
};

inline std::ostream & operator <<(std::ostream & out, std::vector<Edge> const & edge){
	for(std::size_t k=0; k<edge.size(); k++)
		out << edge[k] << std::endl;
	return out;
};


#endif
