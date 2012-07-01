#include<iostream>
#include"node.h"
#include"rrgraph.h"
#include"model.h"
using namespace std;
int seed=0;
std::tr1::ranlux64_base_01 eng(seed);

int main(int argc, char **argv){

	if(argc>1)seed=atoi(argv[1]);
	eng.seed(seed);

	CNetwork *net=new CRRGraph(100000, 4);

	CModel model(net);
	model.Initial_Conditions();
	model.Iterate();

return 0;

}
