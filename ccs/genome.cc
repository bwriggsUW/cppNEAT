#include "../headers/genome.h"
#include "../headers/connection.h"
#include "../headers/node.h"
#include<random>
#include<vector>
#include<string>
#include<iostream>

Genome::Genome(std::vector<Node*> pNodeGenes, std::vector<Connection*> pConnectionGenes) 
	: nodeGenes(pNodeGenes), connectionGenes(pConnectionGenes){
	numInnovs = pNodeGenes.size() + pConnectionGenes.size();
	numNodes = pNodeGenes.size();
}

Genome::Genome(int numInputs, int numOutputs){
	this->nodeGenes.resize(numInputs + numOutputs);
	this->connectionGenes.resize(numInputs * numOutputs);
	this->numInnovs = 0;
	this->numNodes = 0;
		
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_real_distribution<float> distr(-1.0, 1.0);

	for(int i = 0; i < numInputs + numOutputs; i++){
		this->nodeGenes[i] = new Node(this->numNodes++, i < numInputs ? Node::Type::INPUT : Node::Type::OUTPUT, this->numInnovs++);
	}
	for(int i = 0; i < numInputs; i++){
		for(int j = 0; j < numOutputs; j++){
			this->connectionGenes[i * numOutputs + j] = new Connection(i, numInputs + j, distr(gen), true, this->numInnovs++);
		}
	}	
}

Genome::~Genome(){
	for(int i = 0; i < this->nodeGenes.size(); i++) delete this->nodeGenes[i];
	for(int i = 0; i < this->connectionGenes.size(); i++) delete this->connectionGenes[i];
}

void Genome::addNode(){
	std::vector<int> enabledIndices;
	int connectionToSplit;
	for(int i = 0; i < this->connectionGenes.size(); i++){
		if(this->connectionGenes[i]->enabled){
			enabledIndices.push_back(i);
		}
	}

	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> distr(0, enabledIndices.size() - 1);

	connectionToSplit = distr(gen);
	int from, to;
	float weight;
	from = this->connectionGenes[connectionToSplit]->fromId;
	to = this->connectionGenes[connectionToSplit]->toId;
	weight = this->connectionGenes[connectionToSplit]->weight;

	this->connectionGenes[connectionToSplit]->enabled = false;
	this->nodeGenes.push_back(new Node(this->numNodes++, this->numInnovs++));
	this->connectionGenes.push_back(new Connection(from, this->numNodes - 1, 1.0, true, this->numInnovs++));
	this->connectionGenes.push_back(new Connection(this->numNodes - 1, to, weight, true, this->numInnovs++));
}

void Genome::addConnection(){
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> distr1(0, this->numNodes - 1);
	std::uniform_int_distribution<int> distr2(0, this->numNodes - 2);
	std::uniform_real_distribution<float> distr3(-1.0, 1.0);
	
	int node1 = distr1(gen);
	int node2 = distr2(gen);
	if(node2 >= node1) node2++;

	this->connectionGenes.push_back(new Connection(node1, node2, distr3(gen), true, this->numInnovs++));
}

int max(int a, int b){
	return a > b ? a : b;
}

std::string type2Str(Node::Type type){
	return type == Node::Type::INPUT ? "INPUT" : type == Node::Type::HIDDEN ? "HIDDEN" : "OUTPUT";
}

std::string alignLeft(int width, std::string str){
	return str + std::string(width - str.length(), ' ');
}

void Genome::info(){
	int unitWidth = 25;
      	int maxWidth = max(this->nodeGenes.size(), this->connectionGenes.size());
	std::cout << std::string(maxWidth * unitWidth, '=') << std::endl;
	std::cout << "Nodes:" << std::endl;
	std::vector<std::string> curLines(3, "");
	for(int i = 0; i < this->nodeGenes.size(); i++){
		curLines[0] = curLines[0] + alignLeft(unitWidth, "innovNum: " + std::to_string(this->nodeGenes[i]->innovNum));
		curLines[1] = curLines[1] + alignLeft(unitWidth, "id: " + std::to_string(this->nodeGenes[i]->id));
		curLines[2] = curLines[2] + alignLeft(unitWidth, "type: " + type2Str(this->nodeGenes[i]->type));
	}
	std::cout << curLines[0] << std::endl << curLines[1] << std::endl << curLines[2] << std::endl;
	std::cout << std::string(maxWidth * unitWidth, '=') << std::endl << "Connections:" << std::endl;
	curLines.resize(5, "");
	curLines[0] = "";
	curLines[1] = "";
	curLines[2] = "";
	for(int i = 0; i < this->connectionGenes.size(); i++){
		curLines[0] = curLines[0] + alignLeft(unitWidth, "innovNum: " + std::to_string(this->connectionGenes[i]->innovNum));
		curLines[1] = curLines[1] + alignLeft(unitWidth, "fromId: " + std::to_string(this->connectionGenes[i]->fromId));
		curLines[2] = curLines[2] + alignLeft(unitWidth, "toId: " + std::to_string(this->connectionGenes[i]->toId));
		curLines[3] = curLines[3] + alignLeft(unitWidth, "weight: " + std::to_string(this->connectionGenes[i]->weight));
		curLines[4] = curLines[4] + alignLeft(unitWidth, this->connectionGenes[i]->enabled ? "ENABLED" : "DISABLED");
	}
	std::cout << curLines[0] << std::endl << curLines[1] << std::endl << curLines[2] 
		<< std::endl << curLines[3] << std::endl << curLines[4] << std::endl;
	std::cout << std::string(maxWidth  * unitWidth, '=') << std::endl;
}

	


