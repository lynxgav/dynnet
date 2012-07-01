#ifndef MODEL_H
#define MODEL_H 
#include"network.h"

double beta=2.5;
double delta=1.;
double gama=2.5;

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
	tmax=6;

	elapsed=0;

	network->TotalProb=0.;

	inf=(int)(0.03003*network->get_N());
	sus=(int)(0.5929*network->get_N());
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

	//cerr<< sus << "\t" << inf << "\t" << rec << endl;

	inf=(int)(0.03003*network->get_N());
	sus=(int)(0.5929*network->get_N());
	rec=network->get_N()-inf-sus;

}

void CModel::UpdateProbabilities(CNode* node){
	network->TotalProb-=node->prob;
	if( node->state==INF ) { node->prob=delta;}
	else if ( node->state==REC ) { node->prob=gama;}
	else if ( node->state==SUS ) { int nc=node->count_neighbours_state(INF); node->prob=beta*nc;}
	else { cerr << "Error in Initial Conditions" << endl; }
	network->TotalProb+=node->prob;
}


CNode * CModel::Choose_Transition(){
	
	std::tr1::uniform_real<double> unif(0, 1);
	elapsed-=log(1.0-unif(eng))/network->TotalProb;
	double chosen=network->TotalProb*unif(eng);

	double p_sum;
	int i;

	for(i=0, p_sum=network->nodes.at(i)->prob; chosen >= p_sum ; i++, p_sum+=network->nodes.at(i)->prob){};

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

	UpdateProbabilities( node );//TotalProb is updated here

	list<CNode*>::iterator it;
	if(node->state!=SUS){
		for(it=node->neighbours.begin(); it!=node->neighbours.end(); it++) {
			if( (*it)->state == SUS ){
				UpdateProbabilities( *it );
			}
		}
	}
}

void CModel::Iterate(){
	while(t<=tmax and inf>0){
		Execute_Transition(Choose_Transition());
		//cerr<< elapsed <<endl;
		if(elapsed >= tstep){
			t+=tstep;
			cout<< t <<"\t"<< sus/(double)network->get_N() <<"\t"<< inf/(double)network->get_N() <<"\t"<< rec/(double)network->get_N() <<endl;
			elapsed-=tstep;
			}
		
	}

}

//git commit -a -m "Initial commit. Very slow"
//git push origin master

#endif /* MODEL_H */
