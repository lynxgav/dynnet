#ifndef MODEL_H
#define MODEL_H 
#include"network.h"

double beta=2.5;
double delta=1.;
double gama=0.1;

enum{SUS=0, INF=1, REC=2};


class CModel
	{
	public:
	CModel(CNetwork *net){
		network=net;
		}

	void UpdateProbabilities(CNode* node);
	void Initial_Conditions();
	CNode * Choose_Transition();
	void Execute_Transition(CNode* node);
	
	void Iterate();
	
	//int Count_State(int start, int end);

	CNetwork *network;
	double t, tstep, tmax;
	double elapsed;
 	private:
};


int inf;
int sus;
int rec;

void CModel::Initial_Conditions(){

	t=0;
	tstep=0.001;
	tmax=100;

	elapsed=0;
	inf=(int)(0.05*network->get_N());
	sus=(int)(0.5*network->get_N());
	rec=network->get_N()-inf-sus;

	ERROR(rec<0, "The sum of infected and susceptibles is larger than total.");

	std::tr1::uniform_real<double> unif(0, 1);

	double s, i, rand;
	int j=network->get_N();
	vector<CNode *>::iterator it;
	//Distribute the state randomly
	for (it=network->nodes.begin(); it!=network->nodes.end(); it++) {
		s=(double)sus/(double)j; i=(double)inf/(double)j;
		rand=unif(eng);
		if (rand < s ) {(*it)->state=SUS; sus--;}
		else if (rand <s+i) {(*it)->state=INF; inf--;}
		else {(*it)->state=REC; rec--;}
		--j;
	}
	
	
	for (it=network->nodes.begin(); it!=network->nodes.end(); it++) {
		UpdateProbabilities( *it );
	}

	inf=(int)(0.05*network->get_N());
	sus=(int)(0.5*network->get_N());
	rec=network->get_N()-inf-sus;

}

void CModel::UpdateProbabilities(CNode* node){
	if( node->state==INF ) { node->prob=delta;}
	else if ( node->state==REC ) { node->prob=gama;}
	else if ( node->state==SUS ) { int nc=node->count_neighbours_state(INF); node->prob=beta*nc;}
	else { cerr << "Error in Initial Conditions" << endl; }
}

double vProb[1000000];
CNode * CModel::Choose_Transition(){
	double p_sum=0;
	vector<CNode *>::iterator it;
	int i=0;
	
	for (it=network->nodes.begin(); it!=network->nodes.end(); it++,i++) {
		vProb[i]=p_sum+=(*it)->prob;
	}
	
	std::tr1::uniform_real<double> unif(0, 1);
	elapsed-=log(1.0-unif(eng))/p_sum;
	p_sum*=unif(eng);

	for(i=0; p_sum>=vProb[i]; i++){};
	return network->nodes.at(i);
}

void CModel::Execute_Transition(CNode* node){

	if( node->state==INF ) {
		inf--; rec++;
		node->state=REC;
	}
	else if ( node->state==REC ) { 
		rec--; sus++;
		node->state=SUS;
	}
	else if ( node->state==SUS ) { 
		ERROR(node->count_neighbours_state(INF) == 0, "Node cannot become infected");
		sus--; inf++;
		node->state=INF;
	}
	else { cerr << "Error in Initial Conditions" << endl; }

	UpdateProbabilities( node );

	list<CNode*>::iterator it;
	if(node->state!=SUS){
		for(it=node->neighbours.begin(); it!=node->neighbours.end(); it++) {
			UpdateProbabilities( *it );
		}
	}
}

void CModel::Iterate(){
	while(t<=tmax and inf>0){
		Execute_Transition(Choose_Transition());
		//cerr<< elapsed <<endl;
		if(elapsed >= tstep){
			t+=tstep;
			cout<< t <<"\t"<< sus <<"\t"<< inf <<"\t"<< rec <<endl;
			elapsed-=tstep;
			}
		
	}

}

//git commit -a -m "Initial commit. Very slow"
//git push origin master

#endif /* MODEL_H */
