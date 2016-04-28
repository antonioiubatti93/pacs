#include "Polygon.hpp"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <limits>

namespace Geometry{
  
  double distance(Point2D const & a, Point2D const & b){
    // Not very efficient. This function should be implemented either
    // as friend or as a method, to access private members.
    Point2D tmp(a-b);
    return std::sqrt(tmp.x()*tmp.x()+
		     tmp.y()*tmp.y()
		     );
  }
  // ********************* BASE CLASS **********************
  
  AbstractPolygon::AbstractPolygon(Vertices const & v, std::vector<Point2D> & pp):vertexes(v),p(nullptr){
	p = std::make_shared<std::vector<Point2D> >(pp);
  }
 
  // ****   POLYGON
  
  Polygon::Polygon(Vertices const & v, std::vector<Point2D> & pp): AbstractPolygon(v,pp) {}
  
  
  //! To compute the area of a polygon we use the divergence theorem.
  /*!
    \f$ int_P d\Omega = 1/2 \int{\partial P} xn_x d\gamma\f$
    The integral is computed by using trapezoidal rule on the polygon sides.
  */
  double Polygon::area() const{
    auto siz=this->size();
    if (siz<3) return 0;
    double result(0);
    Vertices const & Ver(this->vertexes);
    // I am using C++11 sintax. decltype(expr) returns the
    // type of the expression expr. (I could have used Vertices::size_type, 
    // or just int, here I wanted to show some features of the new standard).
    for (decltype(siz) i=0; i<siz;++i){
      // Current point I use references to avoid unnecessary construction
      // of objects
      Point2D const & p1( p ->at(Ver[i]));
      // Other point
      Point2D const & p2( p ->at(Ver[(i+1) % siz])); // indirect access
      Point2D const & p0( p ->at(Ver[(i-1) % siz]));
      result+=p1.x()*(p2.y()-p0.y());
    }
    return 0.5*result;
  }
  

  // ********************* SQUARE **********************
  Square::Square(Vertices const & v, std::vector<Point2D> & pp): AbstractPolygon(v,pp) {}
  
  double Square::area() const{
    if(this->size()==0) return 0.0;
    // I want the area with sign, positive if the quad is 
    // oriented counterclockwise. So the easiest thing is to use the 
    // formula using cross product (even if it is not the most efficient choice)
    Point2D v(this->p->at(vertexes[1])-this->p->at(vertexes[0])); // indirect access through pointer
    Point2D w(this->p->at(vertexes[2])-this->p->at(vertexes[0]));
    // area = v \times w. Positive if square counterclockwise oriented
    return v.x()*w.y()-v.y()*w.x();
    ;}
  
  
  //********************* TRIANGLE **********************
  
  Triangle::Triangle(Vertices const & v, std::vector<Point2D> & pp):AbstractPolygon(v,pp) {}
  

  double Triangle::area() const
  {
    if(this->size()==0) return 0.0;
    // I use the cross product since this is a 
    // signed area!
    Point2D v(this->p->at(vertexes[1])-this->p->at(vertexes[0]));
    Point2D w(this->p->at(vertexes[2])-this->p->at(vertexes[0]));
    // area = 0.5* v \times w. Positive if triangle counterclockwise oriented
    return 0.5*(v.x()*w.y()-v.y()*w.x());
    ;}
  

}
