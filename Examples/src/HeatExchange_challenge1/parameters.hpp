#ifndef HH_Parameters_HH
#define HH_Parameters_HH
#include <iosfwd>
#include <string>
struct parameters
{
  //! max number of iteration for Gauss-Siedel
  int   itermax;
  //! Tolerance for stopping criterion
  double  toler;
  //! Bar length
   double L;
  //! First longitudinal dimension
  double a1;
 //! Second longitudinal dimension
  double a2;
  //! Dirichlet condition
  double To;
  //! External temperature 
  double Te;
  //! Conductivity
  double k;
  //! Convection coefficient
  double hc;
  //! Number of elements
  int M;
  //! CHALLENGE MODIFICATION: name of output file
  std::string outputFileName;
  //! CHALLENGE MODIFICATION: flag to print the output on the terminal
  int printout;
  int printfile;
  //CHALLENGE MODIFICATION: choice of the discrete norm
  std::string norm;
  //CHALLENGE MODIFICATION: choice of the linear system solver
  std::string solver;
  //! Constructor takes default values
  parameters():
    itermax(1000000),
    toler(1e-8),
    L(40.),
    a1(4.),
    a2(50.),
    To(46.),
    Te(20.),
    k(0.164),
    hc(1.e-6*200.),
    M(11),
    //!CHALLENGE MODIFICATION
    outputFileName("result.dat"),
    printout(0),
    printfile(1),
    norm("Euclidean"),
    solver("gauss_seidel")
  {};
};
//! Prints parameters
std::ostream & operator << (std::ostream &,const parameters &);
#endif
