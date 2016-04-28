#ifndef EDGE_CLASS_HPP
#define EDGE_CLASS_HPP

#include <array>
#include <iostream>

using pointindex = std::array<unsigned int,2>;

class Edge {
	public:
	Edge() = default;
	Edge(unsigned int a, unsigned int b): A(a), B(b) {};//constructor initializes an object to the null Edge: coinciding points
	Edge(Edge const &) = default;
	Edge & operator =(Edge const &) = default;
	~Edge() = default;
	//specific methods
	void set(unsigned int a, unsigned int b);
	pointindex get() const;
	friend std::ostream & operator <<(std::ostream &, Edge const &);
	friend bool operator < (Edge const &, Edge const &);
	private:
	unsigned int A;
	unsigned int B;
};

inline void Edge::set(unsigned int a, unsigned int b){
	A = a; B = b;
};

inline pointindex Edge::get() const {
	pointindex v = {A,B};
	return v;
};

inline std::ostream & operator << (std::ostream & out, Edge const & E){
	pointindex v(E.get());
	out << "(" << v[0] << "," << v[1] << ")";
	return out;
};

/* very peculiar (and arbitrary) ordering relation
an edge is not ordered by itself: exchanging two points
does not affect the ordering!!! */

inline bool operator <(Edge const & e1, Edge const & e2){
	pointindex v1(e1.get()), v2(e2.get());
	if(v1[0]+v1[1] == v2[0]+v2[1]){
		if((v1[0] == v2[0] && v1[1] == v2[1])||(v1[0] == v2[1] && v1[0] == v2[1]))
			return false;
		else {
			if(v1[0] == v2[0])
				return v1[1] < v2[1];
			return v1[0] < v2[0];
		}
	}
	return v1[0]+v1[1] < v2[0] + v2[1];
};

#endif
