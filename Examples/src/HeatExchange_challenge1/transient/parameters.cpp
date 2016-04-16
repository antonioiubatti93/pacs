#include <iostream>
#include "parameters.hpp"
std::ostream & operator << (std::ostream & out,const parameters & p)
{
  out<<"PARAMETER VALUES:"<<"\n";
  out<<"itermax= "<<p.itermax<<"\n";
  out<<"toler= "<<p.toler<<"\n";
  out<<"L= "<<p.L<<"\n";
  out<<"a1= "<<p.a1<<"\n";
  out<<"a2= "<<p.a2<<"\n";
  out<<"T= "<<p.T<<"\n";
  out<<"To= "<<p.To<<"\n";
  out<<"Te= "<<p.Te<<"\n";
  out<<"k= "<<p.k<<"\n";
  out<<"hc= "<<p.hc<<"\n";
  out<<"rho*cv= "<<p.rhocv<<"\n";
  out<<"M= "<<p.M<<"\n";
  out<<"N= "<<p.N<<"\n";
//! CHALLENGE MODIFICATION
  out<<"Output file name: "<<p.outputFileName<<"\n";
  out<<"Printfile option: "<<p.printfile<<"\n";
  out<<"Printout option: "<<p.printout<<"\n";
  out<<"Norm option: "<<p.norm<<"\n";
  out<<"Linear system solver: "<<p.solver<<"\n";
  out<<"Time scheme: "<<p.timescheme<<"\n\n";
  return out;
}
