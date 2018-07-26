#ifndef __GENOME_H__
#define __GENOME_H__

#include "node.h"
#include "connection.h"
#include<vector>

class Genome{
	public:
		Genome(std::vector<Node*> pNodeGenes, std::vector<Connection*> pConnectionGenes);
		Genome(int numInputs, int numOutputs);
		~Genome();

		void addNode();
		void addConnection();
		void stdMutateWeight(float min, float max);
		void rndMutateWeight();

		float feedForward();

		void info();

		static Genome crossover(Genome& moreFitParent, Genome& lessFitParent);
	private:
		std::vector<Node*> nodeGenes;
		std::vector<Connection*> connectionGenes;
		int numInnovs;
		int numNodes;

};

#endif
