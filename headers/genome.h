#ifndef __GENOME_H__
#define __GENOME_H__

#include "node.h"
#include "connection.h"
#include<vector>
#include<random>

class Genome{
	public:
		Genome(std::vector<Connection*> pConnectionGenes, std::vector<Node*> pNodeGenes, std::default_random_engine& pGen);
		Genome(const Genome& moreFitParent, const Genome& lessFitParent, bool equalFitness); 
		Genome(int numInputs, int numOutputs, std::default_random_engine& pGen);
		~Genome();

		float feedForward(std::vector<float> input);
		void info();

		int posOfRndCon(ConnectionDesc* cd);
		void rndCon(ConnectionDesc* cd);
		void mutateAddNode(int connectionToSplit, int innovNum);
		void mutateAddConnection(ConnectionDesc* cd, int innovNum);
		void stdMutateWeight(float min, float max);
		void rndMutateWeight();
		
		static float compatibility(const Genome& genome1, const Genome& genome2, float c1, float c2, float c3);
	private:
		std::vector<Node*> nodeGenes;
		std::vector<Connection*> connectionGenes;
		std::default_random_engine& gen;
		int highestNode;
};

#endif
