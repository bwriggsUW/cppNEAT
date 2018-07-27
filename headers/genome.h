#ifndef __GENOME_H__
#define __GENOME_H__

#include "node.h"
#include "connection.h"
#include<vector>
#include<random>

class Genome{
	public:
		Genome(std::vector<Node*> pNodeGenes, std::vector<Connection*> pConnectionGenes, std::default_random_engine& pGen);
		Genome(int numInputs, int numOutputs, std::default_random_engine& pGen);
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
		std::default_random_engine gen;
};

#endif
