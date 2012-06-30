#ifndef RRGRAPH_H
#define RRGRAPH_H 

#include"network.h"


class CRRGraph : public CNetwork
	{
	public:
	CRRGraph(unsigned int n, unsigned int k);
	bool generate(unsigned int n, unsigned int k);
	unsigned int K;
 	private:
	};

CRRGraph::CRRGraph(unsigned int n, unsigned int k){
	N=n;
	K=k;
	ERROR(K>=N, "Number of links exceeds that of the complete graph")
	ERROR(is_odd(K) and is_odd(N), "Either N or K should be even");
	while(!generate(N, K)){};

}

bool CRRGraph::generate(unsigned int n, unsigned int k){

	clear();
	N=n;
	K=k;

	vector<CNode *> temp;
	for(unsigned int i=0; i<N; i++){
		CNode *p_node=new CNode(i);
		nodes.push_back(p_node);	
		temp.push_back(p_node);	
		}

	unsigned int nlinks=N*K/2;
	while(nlinks>0){	
		unsigned int tempN=temp.size();
		if(tempN==1)return false; //Not possible to add more links, only one node left
		std::tr1::uniform_int<unsigned int> unif(0, tempN-1);

		unsigned int pos1,pos2;
		int attempt=0;

		pos1=unif(eng);
		do{
			pos2=unif(eng);
			attempt++;
			WARNING(attempt>50,stringify(pos1)+"  "+stringify(pos2));
			if(attempt>50) return false; //Cannot find suitable pairs of nodes

			} while(pos1==pos2);

		bool success1=temp.at(pos1)->add_neighbour(temp.at(pos2), true);
		bool success2=temp.at(pos2)->add_neighbour(temp.at(pos1), true);
		assert(success1==success2);
		if(success1==false)continue;//the link already exists
		if(temp.at(pos1)->degree == K) {
			temp.at(pos1)=temp.back();
			if(pos2==tempN-1) pos2=pos1;
			temp.pop_back();
			} 
		if(temp.at(pos2)->degree == K) {
			temp.at(pos2)=temp.back();
			temp.pop_back();
			} 
		nlinks--;
	}
	assert(temp.size()==0);
	assert(nlinks==0);
	return true;//successfully generated
}

#endif /* RRGRAPH_H */
