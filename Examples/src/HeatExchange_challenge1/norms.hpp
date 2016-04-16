#ifndef NORMS_EUCLIDEAN_L2_H1_CPP
#define NORMS_EUCLIDEAN_L2_H1_CPP
#include<vector>
#include<cmath>
#include<string>
#include<iostream>

//CHALLENGE: calculate the error using a user-defined norm (see input file)

class Error{
	private:
	double error;

	void normE(std::vector<double>& u0,std::vector<double>& u1){
		error=0.;
		for(std::size_t k=0;k<u0.size();k++)
			error+=(u0[k]-u1[k])*(u0[k]-u1[k]);
		error=sqrt(error);
	};

	void normL2(std::vector<double>& u0,std::vector<double>& u1,double h){
		error=0.;
		for(std::size_t k=0;k<u0.size()-1;k++)
			error+=h/3*((u0[k+1]-u1[k+1])*(u0[k+1]-u1[k+1])+(u0[k+1]-u1[k+1])*(u0[k]-u1[k])+(u0[k]-u1[k])*(u0[k]-u1[k]));
		error=sqrt(error);
	};

	void normH1(std::vector<double>& u0,std::vector<double>& u1,double h){
		error=0.;
		for(std::size_t k=0;k<u0.size()-1;k++)
			error+=h/3*((u0[k+1]-u1[k+1])*(u0[k+1]-u1[k+1])+(u0[k+1]-u1[k+1])*(u0[k]-u1[k])+(u0[k]-u1[k])*(u0[k]-u1[k]))+1/h*(u0[k+1]-u0[k]-u1[k+1]+u1[k])*(u0[k+1]-u0[k]-u1[k+1]+u1[k]);
		error=sqrt(error);
	};

	public:
	Error(double error=0.):error(error){};

	double get(){
		return error;
	};
	
	void norm(std::vector<double>& u0,std::vector<double>& u1,double h,std::string const & Norm){
		if(Norm=="Euclidean")
			this->normE(u0,u1);
		if(Norm=="L2")
			this->normL2(u0,u1,h);
		if(Norm=="H1")
			this->normH1(u0,u1,h);
	};

	~Error(){};
};

#endif
