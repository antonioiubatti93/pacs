This directory contains the modifications of the previous files as an answer to the first set of challenges. The subdirectory "transient" contains an example of unsteady problem, which is part of the challenge 1.3 (optional). All other points of the challenge (1.1, 1.2 and 1.3, first part) are stored here. 

Challenge 1.1
This folder contains several possible input files. The default one is still "parameters.pot". All of them contain two additional options, called printfile and printout and stored as integers (0 or 1) in the structure parameters. The former tells the program to print the solution on a file, while with the latter one can have the solution printed directly on the screen (including both the quantitative results and the gnuplot graph). There is a third new option, called outputFileName, where to specify the name of the output file as a string.

Challenge 1.2
To generalize the remainder's calculation, I created a simple class Error in the file norms.hpp. It consists in a double and a minimal set of private and public methods. A public one named norm takes as input two vectors (two consecutive partial solutions of a Gauss-Seidel loop), the discretization pace h and a string norm, which is a new parameter in the input file and specifies which norm will be used (the possibilities are "Euclidean", "L2" or "H1"). The public method will call for the private one that suits the norm definition (I did not used templates or inheritance, because the norm type is not known at compile time; probably there is a smarter way to do that...).
Before the Gauss-Seidel loop, I defined an object of type Error and generalized the previous calculation. The cycle is now inside an "if" statement (see Challenge 1.3). I modified two other points of the loop:
1 - I decided to control the error itself and not its square such as before (this is to be consistent with the tolerance passed in the input file);
2 - I changed the Gauss-Seidel iteration using xnew[m-1] instead of theta[m-1], which should be more efficient.
The other input files are orgainzed as it follows:
input1.pot: Gauss-Seidel with Euclidean norm
input2.pot: Gauss-Seidel with L2 norm
input3.pot: Gauss-Seidel with H1 norm
input4pot: LU solver
input5pot: Cholesky solver (LLT factorization)
input6pot: Thomas solver
To compare the effect of the selected norm on the Gauss-Seidel loop, it should be sufficient to type by terminal:
make -f challenge1.2
It will run sequentially the main program with the first three input files and show the results.

Challenge 1.3
Taking a new input parameter, "solver", I specify the linear system solver I want to use in the program. The possibilities are "gauss_seidel", "Thomas", "Cholesky", "LU". The Thomas algorithm is implemented directly, while the last two algorithms are actually managed by the library Eigen/Dense through a factorization of the matrix.

Challenge 1.3 (transient)
I tried here to manage the unsteady case starting from the formulation of the physical problem. The heat equation should now contain other physical parameters, which are the time horizon T and the specific heat per unit volume (/rho*c_v), set to merely conventional values in the input files.
I used only the Thomas algorithm directly with std::vector<double> to avoid memorizing the whole matrix, and perfomed the LU factorization which explicit calculations, according to the time differencing scheme (BE or CN, respectively backward Euler or Crank-Nicolson). If the option printout is set to 1, the gnuplot interface should show the 3d plot of the numerical solution. The results are stored in a .dat file that is organized in order to represent the computational mesh (x coordinates at each time t). The respective input files are backeuler.pot (default) and cranknick.pot.
