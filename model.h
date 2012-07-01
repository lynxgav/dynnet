#ifndef MODEL_H
#define MODEL_H 
#include"network.h"

double beta=2.5;
double delta=1.;
double gama=0.5;
double w=0.5;

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
	void UpdateSystemState(CNode* node, int from, int to);
	
	vector< vector<CNode*> > system_state;
	int nstates;

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

	nstates=3;

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
	
	for(int i=0; i<nstates; i++){
		system_state.push_back( vector<CNode*>() );
	}
	
	for (it=network->nodes.begin(); it!=network->nodes.end(); it++) {
		system_state.at( (*it)->state ).push_back( (*it) );
		(*it)->index=system_state.at( (*it)->state ).size()-1;
		UpdateProbabilities( *it );
	}

	for(int i=0; i<nstates; i++){
		cerr<<system_state.at(i).size()<<endl;
	}

	cerr<< sus << "\t" << inf << "\t" << rec << endl;

	inf=(int)(0.03003*network->get_N());
	sus=(int)(0.5929*network->get_N());
	rec=network->get_N()-inf-sus;

	cerr<< sus << "\t" << inf << "\t" << rec << endl;

}

void CModel::UpdateProbabilities(CNode* node){
	network->TotalProb-=node->prob;
	if( node->state==INF ) { node->prob=delta;}
	else if ( node->state==REC ) { node->prob=gama;}
	else if ( node->state==SUS ) { int nc=node->count_neighbours_state(INF); node->prob=(beta+w)*nc;}
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

void CModel::UpdateSystemState(CNode* node, int from, int to){

	system_state.at(from).at(node->index)=system_state.at(from).back();
	system_state.at(from).at(node->index)->index=node->index;
	system_state.at(from).pop_back();

	node->state=to;
	system_state.at(to).push_back(node);
	node->index=system_state.at(to).size()-1;
}

void CModel::Execute_Transition(CNode* node){

	if( node->state==INF ) {
		inf--; rec++;
		UpdateSystemState(node, INF, REC);
	}
	else if ( node->state==REC ) {
		rec--; sus++;
		UpdateSystemState(node, REC, SUS);
	}
	else if ( node->state==SUS ) {
		ERROR(node->count_neighbours_state(INF) == 0, "Node cannot become infected/break SI link");

		std::tr1::uniform_real<double> unif(0, 1);
		double chosen=(beta+w)*unif(eng);

		if (chosen <= beta){
			sus--; inf++;
			UpdateSystemState(node, SUS, INF);
		}
		else {
			int nc=node->count_neighbours_state(INF);
			std::tr1::uniform_int<int> unif1(1, nc);
			int chosen=unif1(eng);

			list<CNode*>::iterator it; int count=0;
			for(it=node->neighbours.begin(); count!=chosen; it++) {
				if( (*it)->state == INF ){
					count++;
					if(count==chosen){break;}
				}
			}

			bool success1, success2;

			success1=node->remove_neighbour(*it);
			success2=(*it)->remove_neighbour(node);

			if(!success1 || !success2) cerr << "Error in Link Removal" << endl;

			int max=system_state.at(SUS).size();
			std::tr1::uniform_int<long int> unif2(0, max-1);
			int chosen_pos;
			
			do{			
				chosen_pos=unif2(eng);
				CNode* p_chosen_node=system_state.at(SUS).at(chosen_pos);
				success1=node->add_neighbour(p_chosen_node,true);
				success2=p_chosen_node->add_neighbour(node,true);
			}
			while (!success1 || !success2);
		}
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
		if( (unsigned int)(sus+inf+rec)!=network->get_N()) cerr << "Sum of sus, inf and rec is not equal to N" << endl;
		if((unsigned int)sus!=system_state.at(SUS).size() || (unsigned int)inf!=system_state.at(INF).size() || (unsigned int)rec!=system_state.at(REC).size() ) cerr << "Length of vectors of sus, inf and rec are not correct" << endl;
	}

}

//git commit -a -m "Initial commit. Very slow"
//git push origin master

#endif /* MODEL_H */
