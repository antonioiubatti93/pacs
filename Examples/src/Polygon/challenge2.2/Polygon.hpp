#ifndef HH_POLYGON_HH
#define HH_POLYGON_HH
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <memory>

/*!  @file Polygon.hpp 
  @brief This is an example of class hierarchy that
  represents poligonal object.

  @detail This module is wrapped in the namespace `Geometry`. It
  represent a first example of class hyerarchy with public
  inheritance. The class `AbstractPolygon` defines the general public
  interface of all other classes representing polygonal objects.
  
  We make use of some syntax of the C++11 new standard so you need to
  compile with *-std=c++11*
   */
namespace Geometry
{
  

  //! A class that holds 2D points
  /*! It also represents a vector in R2
   */
  class Point2D
  {
  public:
    //! Constructor giving coordinates.
    Point2D(double xx=0.0, double yy=0.0):coor{{xx,yy}}{}
    //! Copy constructor
    Point2D(const Point2D&)=default;
    //! Returns coordinates in a array<double>.
    std::array<double,2> get() const { return coor;}
    //! Sets point coordinates
    void set(double const &xx, double const &yy)
    {
      coor={{xx,yy}};
    }
    //! x coordinate
    double x() const {return coor[0];}
    //! y coordinate
    double y() const {return coor[1];}
    //! Subtraction is implemented as an external friend function
    friend Point2D operator - (Point2D const & a, Point2D const & b);
    //! Addition is implemented as an external friend function
    friend Point2D operator + (Point2D const & a, Point2D const & b);
  private:
    std::array<double,2> coor;
  };

  //! An alias
  using R2Vector=Point2D;

  //! subtraction operator.
  /*!  
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator - (Point2D const & a, Point2D const & b){
    return Point2D(a.coor[0]-b.coor[0],
                   a.coor[1]-b.coor[1]);
  }

  //! Addition operator.
  /*!  
    It is defined in the header file because I want it to be
    inline.
  */
  inline Point2D operator + (Point2D const & a, Point2D const & b){
    return Point2D(a.coor[0]+b.coor[0],
                   a.coor[1]+b.coor[1]);
  }

  //! Distance between points
  double distance(Point2D const & a, Point2D const & b);
 
  //! Now polygon vertices are vectors of indexes (unsigned integers).
  using Vertices=std::vector<std::size_t>;

  //! Defines the common interface of polygons.
  class AbstractPolygon
  {
  public:
    //! Constructor taking vertices
    AbstractPolygon(Vertices const & v, std::vector<Point2D> & pp);
    //! Default constructor is defaulted
    /*! 
      It is up to the derived classes to fill the vertexex and other info correctly
    */
    AbstractPolygon()=default;
    //! Assignment
    AbstractPolygon & operator=(AbstractPolygon const&)=default;
    //! Copy constructor
    AbstractPolygon(AbstractPolygon const &)=default;
    //! Move constructor
    AbstractPolygon(AbstractPolygon&&)=default;
    //! Move constructor
    AbstractPolygon & operator=(AbstractPolygon&&)=default;
    //! virtual destructor
    virtual ~AbstractPolygon(){};
    //! Returns the number of vertices.
    /*!  We return Vertices::size_type and not just int because
      size_type is guaranteed to be the correct type for indexes in
      stl vectors. Its actual type may be implementation dependent.
      In this case, however, int would have been fine (size_type is
      guaranteed to be an integral type, more precisely
      a type convertible to unsigned int).
    */
    Vertices::size_type size() const {return vertexes.size();}
    Vertices const & theVertices()const {return vertexes;}
    //! The area of the polygon (with sign!).
    /*!
      It is a pure virtual function.
      The implementation is left to the derived classes.
    */
    virtual double area() const=0;

  protected:
// CHALLENGE MODIFICATION: vector of indexes and bare pointer to the vector of points, stored in a Grid object
    std::vector<std::size_t> vertexes;
    std::vector<Point2D>* p;
  };

  //! Class for a generic Polygon
  /*
    A generic Polygon is defined by a set of Vertices which are
    provided by the user
   */
  class Polygon: public AbstractPolygon
  {
  public:
    //! Default constructor.
    //! Polygon may be constructed giving Vertices;
    Polygon(Vertices const & v, std::vector<Point2D> & pp);
    //! Destructor
    virtual ~Polygon(){};
    /*!
      The area is positive if vertices are given in 
      counterclockwise order
    */
    virtual double area() const;
  };

  //! A square
  /*!
    The square is a final class derived from polygon.
   */
  class Square final: public AbstractPolygon
  {
  public:
    Square(Vertices const & v, std::vector<Point2D> & pp);
    //!Special constructor valid only for squares.
    /*!
      /param origin Point which gives the first vertex of the square.
      /param length The length of the side.
      /param angle In radians, tells how the square is  rotated. 
     */
    Square(Square const &)=default;
    Square(Square&&)=default;
    Square & operator=(const Square &)=default;
    Square & operator=(Square &&)=default;
    //! Specialised version for squares
    double area() const;
  };
  
  //! A triangle
  class Triangle final: public AbstractPolygon
  {
  public:
    Triangle(Vertices const &, std::vector<Point2D> & pp);
    Triangle(Triangle const &)=default;
    Triangle(Triangle&&)=default;
    Triangle & operator=(const Triangle &)=default;
    Triangle & operator=(Triangle &&)=default;
    //! Specialised for Triangles
    virtual double area() const;
  };
  
}

#endif
