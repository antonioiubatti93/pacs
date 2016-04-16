#ifndef HH_Parameters_HH
#define HH_Parameters_HH
#include <iosfwd>
#include <string>
struct parameters
{
  //! max number of iteration for iterative solvers
  int   itermax;
  //! Tolerance for stopping criterion
  double  toler;
  //! Bar length
   double L;
  //! First longitudinal dimension
  double a1;
 //! Second longitudinal dimension
  double a2;
  // Time horizon
  double T;
  //! Dirichlet condition
  double To;
  //! External temperature 
  double Te;
  //! Conductivity
  double k;
  //! Convection coefficient
  double hc;
  // Specific heat per unit volume
  double rhocv;
  //! Number of elements
  int M;
  // Number of time steps
  int N;
  //! CHALLENGE MODIFICATION: name of output file
  std::string outputFileName;
  //!  Flag to print the output on the file
  int printfile;
  //! Flag to print the graphical results on the screen
  int printout;
  //Choice of the discrete norm (iterative methods)
  std::string norm;
  //Choice of the linear system solver
  std::string solver;
  //! Constructor takes default values
  std::string timescheme;
  parameters():
    itermax(1000000),
    toler(1e-8),
    L(40.),
    a1(4.),
    a2(50.),
    T(5.),
    To(46.),
    Te(20.),
    k(0.164),
    hc(1.e-6*200.),
    rhocv(1.),
    M(20),
    N(20),
    //!CHALLENGE MODIFICATION
    outputFileName("result.dat"),
    printfile(1),
    printout(1),
    norm("Euclidean"),
    solver("Thomas"),
    timescheme("BE")
  {};
};
//! Prints parameters
std::ostream & operator << (std::ostream &,const parameters &);
#endif
