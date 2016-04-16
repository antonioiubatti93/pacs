#include <iostream> // input output
#include <cmath> // (for sqrt)
#include <vector>
#include <tuple>
#include "readParameters.hpp"
#include "GetPot.hpp"
#include "gnuplot-iostream.hpp"// interface with gnuplot
#include "norms.hpp" //CHALLENGE MODIFICATION: choice of the norm
#include <Eigen/Dense> //CHALLENGE MODIFICATION: choice of the solver
/*!
  @file main.cpp
  @brief Temperature distribution in a 1D bar.

  @detail
    We solve  \f$ -T^{\prime\prime}(x)+act*(T(x)-T_e)=0, 0<x<L \f$ with 
    boundary conditions \f$ T(0)=To; T^\prime(L)=0\f$
    
    **************************************************
    Linear finite elements
    Iterative resolution by Gauss Siedel.
    **************************************************
    
    Example adapted by Luca Formaggia from  a code found in 
    "Simulation numerique an C++" di I. Danaila, F. Hecht e
    O. Pironneau.
*/
//! helper function
void printHelp()
{
  std::cout<<"USAGE: main [-h] [-v] -p parameterFile (default: parameters.pot)"<<std::endl;
  std::cout<<"-h this help"<<std::endl;
  std::cout<<"-v verbose output"<<std::endl;
  std::cout<<"In input file:"<<std::endl;
  std::cout<<"Switch on printfile option to have solution printed on file"<<std::endl;
  std::cout<<"Specify outputFileName"<<std::endl;
  std::cout<<"Switch on printout to see the solution on the terminal"<<std::endl;
  std::cout<<"Specify discrete norm for error controll (gauss_seidel loop). Available norms:"<<std::endl;
  std::cout<<"Euclidean"<<std::endl;
  std::cout<<"L2"<<std::endl;
  std::cout<<"H1"<<std::endl;
  std::cout<<"Specify linear system solver. Available solvers:"<<std::endl;
  std::cout<<"Thomas"<<std::endl;
  std::cout<<"LU"<<std::endl;
  std::cout<<"Cholesky"<<std::endl;
  std::cout<<"gauss_seidel"<<std::endl;
  std::cout<<"Run separately challenge1.2 to make tests with different norms on the same problem"<<std::endl;

}

//! main program
int main(int argc, char** argv)
{
  using namespace std; // avoid std::
  int status(0); // final program status
  GetPot   cl(argc, argv);
  if( cl.search(2, "-h", "--help") )
    {
      printHelp();
      return 0;
    }
  // check if we want verbosity
  bool verbose=cl.search(1,"-v");

  //CHALLENGE MODIFICATION: perform tests with different stopping criteria
	if(cl.search(1,"-1"))
		std::cout << "First test" << std::endl << "Stopping criterion: Euclidean norm" << std::endl;
	if(cl.search(1,"-2"))
		std::cout << "Second test" << std::endl << "Stopping criterion: L2 norm" <<   std::endl;
	if(cl.search(1,"-3"))
		std::cout << "Third test" << std::endl << "Stopping criterion: H1 norm" << std::endl;
  // Get file with parameter values
  string filename = cl.follow("parameters.pot","-p");
  cout<<"Reading parameters from "<<filename<<std::endl;
  // read parameters
  const parameters param=readParameters(filename,verbose);
  // Transfer parameters to local variables
  // I use references to save memory (not really an issue here, it is just
  // to show a possible  use of references)
  const int&    itermax= param.itermax;   //max number of iteration for Gauss-Siedel
  const double& toler=param.toler;   // Tolerance for stopping criterion
  // Here I use auto (remember that you need const and & if you want constant references)
  const auto& L= param.L;  // Bar length
  const auto& a1=param.a1; // First longitudinal dimension
  const auto& a2=param.a2; //  Second longitudinal dimension
  const auto& To=param.To; // Dirichlet condition
  const auto& Te=param.Te; // External temperature (Centigrades)
  const auto& k=param.k;  // Thermal conductivity
  const auto& hc=param.hc; // Convection coefficient
  const auto&    M=param.M; // Number of grid elements
//CHALLENGE MODIFICATION
  const auto& outputFileName=param.outputFileName; // Name of output file
//CHALLENGE MODIFICATION
  const auto& printout=param.printout; // Flag to print the solution on the screen
//CHALLENGE MODIFICATION
  const auto& printfile=param.printfile; // Flag to print the solution on a file named as set in the input parameters file
//CHALLENGE MODIFICATION: Choice of the discrete norm on I=[a,b]
  const auto& norm=param.norm;
//CHALLENGE MODIFICATION: choice of the linear system solver
  const auto& solver=param.solver;

  //! Precomputed coefficient for adimensional form of equation
  const auto act=2.*(a1+a2)*hc*L*L/(k*a1*a2);

  // mesh size
  const auto h=1./M;

  // Solution vector
  std::vector<double> theta(M+1);

  // CHALLENGE MODIFICATION: choice of the solver

  if(solver=="gauss_seidel")
  {
  // Gauss Siedel is initialised with a linear variation
  // of T
  
  for(unsigned int m=0;m <= M;++m)
     theta[m]=(1.-m*h)*(To-Te)/Te;
  
  // Gauss-Seidel
  //CHALLENGE MODIFICATION
  // epsilon=||x^{k+1}-x^{k}|| (Euclidean), L2(I) or H1(I)
  // Stopping criteria epsilon<=toler1

  //CHALLENGE MODIFICATION: user-defined norm on the interval I=[a,b]
  //IMPORTANT: the error is dimensionally intended as sqrt[sum_m(theta[m]^2)], so
  //the convergence is slower, but the result is more accurate
  //corrected the Gauss-Seidel iteration inside the loop, now it's faster

  vector<double> xnew(M+1);
  xnew = theta;
  int iter=0; 
  Error epsilon(0.);
     do
       { // first M-1 row of linear system
        for(int m=1;m < M;m++)
	  xnew[m]  = (xnew[m-1]+theta[m+1])/(2.+h*h*act);
	//Last row
	xnew[M] = xnew[M-1]; 
	epsilon.norm(theta,xnew,h,norm);
	theta=xnew;
	iter=iter+1;     
       }while( (epsilon.get() > toler) && (iter < itermax) );

    if(iter<itermax)
      cout << "M="<<M<<"  Convergence in "<<iter<<" iterations"<<endl;
    else
      {
	cerr << "NOT CONVERGING in "<<itermax<<" iterations "<<
	  "||dx||="<<epsilon.get()<<endl;
	status=1;
      }
  }

  //LU and LLT factorization implemented with Eigen (dense matrix class)

  else
  {
  Eigen::VectorXd xx(M); //Eigen object to store the solution
  Eigen::MatrixXd A(M,M);
  Eigen::Matrix2d Aloc;
  Aloc(0,0)=Aloc(1,1)=1.+act*h*h/2.; //local and global matrix initialization
  Aloc(0,1)=Aloc(1,0)=-1.;
  for(std::size_t i=0; i<M; i++){
	for(std::size_t j=0; j<M; j++)
		A(i,j)=0.;
  }
  for(std::size_t k=0; k<M-1; k++){ //Global matrix construction with block syntax
	A.block(k,k,2,2)+=Aloc;
  }
  A(0,0)+=1.+act*h*h/2.;
  A(M-1,M-1)=1.;//last row: Neumann condition
  Eigen::VectorXd b(M); //known term: null vector
  b(0)=(To-Te)/Te;
  for(std::size_t k=1; k<M; k++)
	b(k)=0;
  //choice of the solver
  if(solver=="LU"){
	xx = A.lu().solve(b);
	theta[0]=(To-Te)/Te; //Dirichlet condition at x=0
	for(std::size_t k=0; k<M; k++)
		theta[k+1]=xx(k);
  }
  if(solver=="Cholesky"){
	xx = A.llt().solve(b);
	theta[0]=(To-Te)/Te; //Dirichlet condition at x=0
	for(std::size_t k=0; k<M; k++)
		theta[k+1]=xx(k);
  }
  
  if(solver=="Thomas"){ //Thomas algorithm
	std::vector<double> D(M);
	std::vector<double> L(M-1);
	std::vector<double> U(M-1);
	std::vector<double> yy(M);
	D[0]=A(0,0);
	for(std::size_t k=0; k<M-1; k++){
		U[k]=A(k,k+1);
		L[k]=A(k+1,k)/D[k];
		D[k+1]=A(k+1,k+1)-L[k]*U[k];
	}
	yy[0]=b(0);
	for(std::size_t k=1; k<M; k++)
		yy[k]=b(k)-L[k-1]*yy[k-1];
	xx(M-1)=yy[M-1]/D[M-1];
	for(std::size_t k=2; k<=M; k++)
		xx(M-k)=(yy[M-k]-U[M-k]*xx(M-k+1))/D[M-k];
	theta[0]=(To-Te)/Te; //Dirichlet condition at x=0
	for(std::size_t k=0; k<M; k++)
		theta[k+1]=xx(k);

  }

  }


 // Analitic solution

    vector<double> thetaa(M+1);
     for(int m=0;m <= M;m++)
       thetaa[m]=Te+(To-Te)*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

     // writing results with format
     // x_i u_h(x_i) u(x_i) and lauch gnuplot 

     Gnuplot gp;
     std::vector<double> coor(M+1);
     std::vector<double> sol(M+1);
     std::vector<double> exact(M+1);

     for(int m = 0; m<= M; m++)             
             // An example of use of tie and tuples!     
             std::tie(coor[m],sol[m],exact[m])=
	    	   std::make_tuple(m*h*L,Te*(1.+theta[m]),thetaa[m]);

//CHALLENGE MODIFICATION: flag to print the solution on a file
     if(printfile){
	     cout<<"Result file: " << outputFileName <<endl;
	     ofstream f(outputFileName);
	     for(int m = 0; m<= M; m++)
		  f<<m*h*L<<"\t"<<Te*(1.+theta[m])<<"\t"<<thetaa[m]<<endl;
             f.close();
     }
// CHALLENGE MODIFICATION: print on the screen option
     if(printout){
	std::cout << "x(i)" << " " << "uh(i)" << " " << "u(i)" << std::endl;
	for(std::size_t m=0; m<=M; m++)
		std::cout << coor[m] << " " << sol[m] << " " << exact[m]    <<   std::endl;
        // Using temporary files (another nice use of tie)
     gp<<"plot"<<gp.file1d(std::tie(coor,sol))<<
       "w lp title 'uh',"<< gp.file1d(std::tie(coor,exact))<<
       "w l title 'uex'"<<std::endl;
     }

     return status;
}
