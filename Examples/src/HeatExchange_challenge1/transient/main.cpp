#include <iostream> // input output
#include <cmath> // (for sqrt)
#include <vector>
#include <tuple>
#include "readParameters.hpp"
#include "GetPot.hpp"
#include "gnuplot-iostream.hpp"

// EXTENSION OF THE PREVIOUS CODE TO TRANSIENT PROBLEMS

//! helper function
void printHelp()
{
  std::cout<<"USAGE: main [-h] [-v] -p parameterFile (default: backeuler.pot)"<<std::endl;
  std::cout<<"-h this help"<<std::endl;
  std::cout<<"-v verbose output"<<std::endl;
  std::cout<<"Available system solvers: Thomas"<<std::endl;
  std::cout<<"Available time differencing schemes:"<<std::endl<<"BE (Backward Euler)"<<std::endl<<"CN (Crank-Nicolson)"<<std::endl;
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

  // Get file with parameter values
  string filename = cl.follow("backeuler.pot","-p");
  cout<<"Reading parameters from "<<filename<<std::endl;
  // read parameters
  const parameters param=readParameters(filename,verbose);
  // Transfer parameters to local variables
  // I use references to save memory (not really an issue here, it is just
  // to show a possible  use of references)
  // Here I use auto (remember that you need const and & if you want constant references)
  const auto& L= param.L;  // Bar length
  const auto& a1=param.a1; // First longitudinal dimension
  const auto& a2=param.a2; //  Second longitudinal dimension
  const auto& T=param.T; // Time horizon of the problem
  const auto& To=param.To; // Dirichlet condition
  const auto& Te=param.Te; // External temperature (Centigrades)
  const auto& k=param.k;  // Thermal conductivity
  const auto& hc=param.hc; // Convection coefficient
  const auto& rhocv=param.rhocv; // Speficic heat per unit volume
  const auto&    M=param.M; // Number of grid elements
  const auto& N=param.N; // Number of time steps

  const auto& outputFileName=param.outputFileName; // Name of output file

  const auto& printfile=param.printfile; // Flag to print the solution on a file named as set in the input parameters file
  const auto& printout=param.printout; // Flag to print the 3d solution on the screen

  const auto& norm=param.norm;

  const auto& solver=param.solver;

  const auto& timescheme=param.timescheme;

  //! Precomputed coefficient for adimensional form of equation
  const auto r=2.*(a1+a2)*hc*T/(rhocv*a1*a2); // reaction coefficient
  const auto D=k*T/(rhocv*L*L); // diffusion coefficient

  // adimensional equation is:
  // du/dt-D*d2u/dx2+ru = 0 in (0,1)x(0,1), with u(y,t)=(theta(Ly,Tt)-Te)/Te

  // mesh size
  const auto h=1./M;

  // time pace
  const auto dt=1./N;

  // Solution vector
  std::vector<double> U((N+1)*(M+1));  // global solution for all time instants
  std::vector<double> u0(M),u(M); // temporary old and new solution in space
  std::vector<double> yy(M); // auxiliary solution vector (intermediate of Thomas algorithm)

  std::vector<double> Adiag(M);
  std::vector<double> Asub(M-1);
  std::vector<double> Aup(M-1);
  std::vector<double> B(M); //Dirichlet condition -> M unknowns

  if(timescheme=="BE")
  {
  for(std::size_t k=0; k<M-1; k++){
	Adiag[k]=dt*D/h*2.+(1.+r*dt)*h; // Backward Euler
	Asub[k]=Aup[k]=-dt*D/h;
	B[k]=h;
  }
  Asub[M-2]=-1.;
  Adiag[M-1]=1.; //Neumann condition at x = 1
  B[M-1]=0;

  U[0]=0.;
  for(std::size_t k=1; k<M+1; k++){
	U[k]=0.;
	u0[k-1]=0.; // initial condition of the problem
  }

  // LU (sparse) factorization of the matrix A -> Thomas algorithm

  std::vector<double> DIAG(M);
  std::vector<double> SUBDIAG(M-1);
  std::vector<double> UPDIAG(M-1);
  DIAG[0]=Adiag[0];
  for(std::size_t k=0; k<M-1; k++){
	UPDIAG[k]=Aup[k];
	SUBDIAG[k]=Asub[k]/DIAG[k];
	DIAG[k+1]=Adiag[k+1]-UPDIAG[k]*SUBDIAG[k];
  }


  for(std::size_t n=1; n<=N; n++)
  {  // Thomas algorithm in space
	yy[0]=B[0]*u0[0]+dt*D*(To-Te)/(h*Te); //Dirichlet condition at x = 0 -> known term
	for(std::size_t m=1; m<M; m++)
		yy[m]=B[m]*u0[m]-SUBDIAG[m-1]*yy[m-1];
	u[M-1]=yy[M-1]/DIAG[M-1];
	for(std::size_t m=2; m<=M; m++)
		u[M-m]=(yy[M-m]-UPDIAG[M-m]*u[M-m+1])/DIAG[M-m];
	U[(M+1)*n]=(To-Te)/Te;
	for(std::size_t m=1; m<=M; m++)
		U[(M+1)*n+m]=u[m-1]; // Update of the global solution
	u0 = u; // Update of the old solution
  
  }
  }

  if(timescheme=="CN")
  {
  std::vector<double> Bsub(M-1),Bup(M-1);
  for(std::size_t k=0; k<M-1; k++){
	Adiag[k]=dt*D/h+(1.+r*dt/2.)*h; // Crank-Nicolson
	Asub[k]=Aup[k]=-dt/2.*D/h;
	B[k]=(1.-r*dt/2.)*h-dt*D/h; // ATTENTION
	Bsub[k]=Bup[k]=dt/2.*D/h;
  }
  Asub[M-2]=-1.;
  Adiag[M-1]=1.; //Neumann condition at x = 1
  B[M-1]=0.;
  Bsub[M-2]=0.;

  U[0]=0.;
  for(std::size_t k=1; k<M+1; k++){
	U[k]=0.;
	u0[k-1]=0.; // initial condition of the problem
  }

  // LU (sparse) factorization of the matrix A -> Thomas algorithm

  std::vector<double> DIAG(M);
  std::vector<double> SUBDIAG(M-1);
  std::vector<double> UPDIAG(M-1);
  DIAG[0]=Adiag[0];
  for(std::size_t k=0; k<M-1; k++){
	UPDIAG[k]=Aup[k];
	SUBDIAG[k]=Asub[k]/DIAG[k];
	DIAG[k+1]=Adiag[k+1]-UPDIAG[k]*SUBDIAG[k];
  }


  for(std::size_t n=1; n<=N; n++)
  {  // Thomas algorithm in space
	yy[0]=B[0]*u0[0]+Bup[1]*u0[1]+dt*D*(To-Te)/(h*Te); //Dirichlet condition at x = 0 -> known term
	for(std::size_t m=1; m<M-1; m++)
		yy[m]=Bsub[m-1]*u0[m-1]+B[m]*u0[m]+Bup[m+1]*u0[m+1]-SUBDIAG[m-1]*yy[m-1];
        yy[M-1]=Bsub[M-2]*u0[M-2]+B[M-1]*u0[M-1]-SUBDIAG[M-2]*yy[M-2];
	u[M-1]=yy[M-1]/DIAG[M-1];
	for(std::size_t m=2; m<=M; m++)
		u[M-m]=(yy[M-m]-UPDIAG[M-m]*u[M-m+1])/DIAG[M-m];
	U[(M+1)*n]=(To-Te)/Te;
	for(std::size_t m=1; m<=M; m++)
		U[(M+1)*n+m]=u[m-1]; // Update of the global solution
	u0 = u; // Update of the old solution
  
  }

  }

     // writing results with format
     // x_i t_i u_h(x_i)
	
  if(printfile){
	  cout<<"Result file: " << outputFileName <<endl;
 	  ofstream f(outputFileName);     
	  for(std::size_t n = 0; n<= N; n++){
		for(std::size_t m=0; m<=M; m++)
			f<<m*h*L<<"\t"<<n*dt*T<<"\t"<<Te*(1.+U[(M+1)*n+m])<<endl;
	  }
          f.close();
  }
  
  if(printout)
  {
  Gnuplot gp;
  gp << "set dgrid3d " << M+1 << "," << N+1 << std::endl;
  gp << "set hidden3d" << std::endl;
  gp << "set pm3d" << std::endl;
  gp << "splot '" << outputFileName << "' u 1:2:3 w l title 'uh' " << std::endl;
  }

  return status;
}
