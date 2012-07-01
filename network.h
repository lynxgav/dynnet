#ifndef NETWORK_H
#define NETWORK_H 
#include<vector>
#include<iostream>
#include<fstream>
#include <tr1/random>
#include <assert.h>

extern int seed;
extern std::tr1::ranlux64_base_01 eng;

using namespace std;

bool is_odd(unsigned int i){
	if(i%2==1)return true;
	return false;
}

class CNetwork
	{
	public:
	~CNetwork();
	vector<CNode *> nodes;
	virtual void print(ostream &out);
	void clear();
	unsigned int get_N(){
		return nodes.size();
		}
	unsigned int N;
	double TotalProb;
 	private:
	};

CNetwork::~CNetwork(){
	clear();
	}

void CNetwork::clear(){
	vector<CNode*>::iterator it;
	for(it=nodes.begin(); it!=nodes.end(); it++){
		delete(*it);
		}
	nodes.clear();
	N=0;
}

void CNetwork::print(ostream &out){
		vector<CNode*>::iterator it;
		for(it=nodes.begin(); it!=nodes.end(); it++){
			out<<(*it)->ID<<":  ";
			(*it)->print_neighbours(out);
			out<<endl;
			}
		}



#endif /* NETWORK_H */
