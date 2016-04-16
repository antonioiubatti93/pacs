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
  out<<"To= "<<p.To<<"\n";
  out<<"Te= "<<p.Te<<"\n";
  out<<"k= "<<p.k<<"\n";
  out<<"hc= "<<p.hc<<"\n";
  out<<"M= "<<p.M<<"\n";
//! CHALLENGE MODIFICATION
  out<<"Output file name: "<<p.outputFileName<<"\n";
  out<<"Print on the screen option: "<<p.printout<<"\n";
  out<<"Print on file option: "<<p.printfile<<"\n";
  out<<"Norm option: "<<p.norm<<"\n";
  out<<"Linear system solver: "<<p.solver<<"\n\n";
  return out;
}
