#ifndef NODE_H
#define NODE_H 
#include<list>
#include<algorithm>
#include"exception.h"
#include<iostream>
#include<fstream>
using namespace std;

class CNode
	{
	public:
	CNode(int id=-1){ID=id; degree=0; prob=0;}
	~CNode();
	list<CNode*> neighbours;
	bool add_neighbour(CNode*, bool trial=false);
	bool remove_neighbour(CNode*, bool trial=false);
	void print_neighbours(ostream &out);
	void clear_neighbourhood();
	int count_neighbours_state(int s);
	int ID;
	unsigned int degree;
	int state;
	double prob;
 	private:
	};

CNode::~CNode(){
	clear_neighbourhood();
}

void CNode::clear_neighbourhood(){
	list<CNode*>::iterator it;
	for(it=neighbours.begin(); it!=neighbours.end(); it++){
		(*it)->remove_neighbour(this);
	}
}

bool CNode::add_neighbour(CNode* p_node, bool trial ){
	ERROR(degree != neighbours.size(), "Degree is not equal to number of neighbours");
	list<CNode*>::iterator it=find(neighbours.begin(), neighbours.end(), p_node);
	ERROR(trial==false && it!=neighbours.end(), "The neighbour already exists");

	if (it!=neighbours.end()) return false;
	neighbours.push_back(p_node);
	degree=neighbours.size();
	return true;// success
}

bool CNode::remove_neighbour(CNode* p_node, bool trial){
	ERROR(degree != neighbours.size(), "Degree is not equal to number of neighbours");
	list<CNode*>::iterator it=find(neighbours.begin(), neighbours.end(), p_node);
	ERROR(trial==false && it==neighbours.end(), "There is no such neighbour to remove");

	if (it==neighbours.end()) return false;
	neighbours.erase(it);
	degree=neighbours.size();
	return true;// success
}

void CNode::print_neighbours(ostream &out){
	list<CNode*>::iterator it;
	for(it=neighbours.begin(); it!=neighbours.end(); it++) {
		out<<(*it)->ID<<"   ";
		}
}

int CNode::count_neighbours_state(int s){
	int count=0;
	list<CNode*>::iterator it;
	for(it=neighbours.begin(); it!=neighbours.end(); it++) {
		if((*it)->state==s)count++;
		}
	return count;
}
#endif /* NODE_H */
