#ifndef LATTICE_H
#define LATTICE_H 
#include"network.h"

class CLattice : public CNetwork
	{
	public:
	CLattice(long int w, long int h);
	void print(ostream &out);
	long int W, H;
 	private:
	vector<CNode *> temp;
	};

CLattice::CLattice(long int w, long int h){
	W=w; H=h; N=H*W;
	for(long int i=0; i<N; i++){
		CNode *p_node=new CNode(i);
		nodes.push_back(p_node);	
		temp.push_back(p_node);	
		}
	long int k;
	for (k=0; k<N; k++)  (abs(k-W*(k/W) )==0) ?  
			temp.at(k)->add_neighbour( temp.at(k+W-1))  : temp.at(k)->add_neighbour( temp.at(abs(k-1))); // da esquerda
	for (k=0; k<N; k++)  (abs(k+1-W*((k+1)/W))==0) ?  
			temp.at(k)->add_neighbour( temp.at(k-W+1))  : temp.at(k)->add_neighbour( temp.at(k+1)); // da direita
	for (k=0; k<N; k++)  k<W ? 
			temp.at(k)->add_neighbour( temp.at(k+H*W-W))  : temp.at(k)->add_neighbour( temp.at(k-W)); // de cima
	for (k=0; k<N; k++)  k>=(W*H-W) ? 
			temp.at(k)->add_neighbour( temp.at(k-H*W+W))  : temp.at(k)->add_neighbour( temp.at(k+W)); // de baixo

	}

void CLattice::print(ostream &out){
		for(long int i=0; i<H; i++){
			for(long int j=0; j<W; j++){
				out<<temp.at(j+i*W)->ID<<"  ";
				}
			out<<endl;
			}
		CNetwork::print(out);
		}

#endif /* LATTICE_H */
