#ifndef GRID_CLASS_HPP
#define GRID_CLASS_HPP
#include <vector>
#include "Polygon.hpp"
#include <iostream>
#include <fstream>
#include <memory>

using namespace Geometry;

class Grid{
	public:
	Grid() = default;
	Grid & operator=(Grid const & grid) = default;
	~Grid() = default;
	Grid(std::string filename);
	double area() const;
	double area();
	private:
	std::vector<Point2D> points;
	std::vector<std::shared_ptr<AbstractPolygon > > elems;
};


#endif
