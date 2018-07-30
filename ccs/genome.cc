#include "../headers/genome.h"
#include "../headers/connection.h"
#include "../headers/node.h"
#include<random>
#include<vector>
#include<string>
#include<iostream>

int max(int a, int b){
	return a > b ? a : b;
}

float abs(float x){
	return (x>0.0 ? x : -1*x);
}

float Genome::compatibility(const Genome& genome1, const Genome& genome2, float c1, float c2, float c3){
	int size1 = genome1.connectionGenes.size();
	int size2 = genome2.connectionGenes.size();

	const std::vector<Connection*>& smaller = (size1 <= size2 ? genome1.connectionGenes : genome2.connectionGenes);
	const std::vector<Connection*>& bigger = (size1 > size2 ? genome1.connectionGenes : genome2.connectionGenes); 
	int smallerSize = size1 < size2 ? size1 : size2;
	int biggerSize = size1 > size2 ? size1 : size2;

	int n = (biggerSize >= 20 ? biggerSize : 1);
	int numSharedGenes = 0;
	int numExcessGenes = 0;
	int numDisjointGenes = 0;
	float totalWeightDiff = 0.0;

	int smallMinInnovNum = smaller[0]->innovNum;
	int smallMaxInnovNum = smaller[smallerSize - 1]->innovNum;
	int bigMinInnovNum = bigger[0]->innovNum;
	int bigMaxInnovNum = bigger[biggerSize - 1]->innovNum;

	int j = 0, k = 0;
	std::vector<int> smallerInnovNums;
	std::vector<int> biggerInnovNums;
	for(;(j < smallerSize) and (k < biggerSize);){
		if(smaller[j]->innovNum == bigger[k]->innovNum){
			numSharedGenes++;
			totalWeightDiff += abs(smaller[j]->weight - bigger[k]->weight);
			smallerInnovNums.push_back(smaller[j++]->innovNum);
			biggerInnovNums.push_back(bigger[k++]->innovNum);
		}else if(smaller[j]->innovNum > bigger[k]->innovNum){
			smallerInnovNums.push_back(-1);
			biggerInnovNums.push_back(bigger[k++]->innovNum);
		}else{
			smallerInnovNums.push_back(smaller[j++]->innovNum);
			biggerInnovNums.push_back(-1);
		}
	}

	numExcessGenes = smallerSize + biggerSize - j - k;
	for(int i = 0; i < smallerInnovNums.size(); i++){
		if(smallerInnovNums[i] == -1){
			if((biggerInnovNums[i] > smallMinInnovNum) and (biggerInnovNums[i] < smallMaxInnovNum)){
				numDisjointGenes++;
			}else{
				numExcessGenes++;
			}
		}else if(biggerInnovNums[i] == -1){
			if((smallerInnovNums[i] > bigMinInnovNum) and (smallerInnovNums[i] < bigMaxInnovNum)){ 
				numDisjointGenes++;
			}else{
				numExcessGenes++;
			}
		}
	}

	return (c1*(float)numExcessGenes + c2*(float)numDisjointGenes)/n + c3*totalWeightDiff/numSharedGenes;
}

Genome::Genome(std::vector<Connection*> pConnectionGenes, std::vector<Node*> pNodeGenes, std::default_random_engine& pGen) 
	: connectionGenes(pConnectionGenes), nodeGenes(pNodeGenes), gen(pGen){}

Genome::Genome(int numInputs, int numOutputs, std::default_random_engine& pGen) : gen(pGen){
	this->nodeGenes.resize(numInputs + numOutputs);
	this->connectionGenes.resize(numInputs * numOutputs);
	this->highestNode = numInputs + numOutputs - 1;
		
	this->gen.discard(10000);
	std::uniform_real_distribution<float> distr(-1.0, 1.0);

	for(int i = 0; i < numInputs + numOutputs; i++){
		this->nodeGenes[i] = new Node(i < numInputs ? Node::Type::INPUT : Node::Type::OUTPUT, i);
	}
	for(int i = 0; i < numInputs; i++){
		for(int j = 0; j < numOutputs; j++){
			this->connectionGenes[i * numOutputs + j] = 
				new Connection(i, numInputs + j, distr(this->gen), true, i * numOutputs + j);
		}
	}	
}

Genome::Genome(const Genome& moreFitParent, const Genome& lessFitParent, bool equalFitness) : gen(moreFitParent.gen){}

Genome::~Genome(){
	for(int i = 0; i < this->nodeGenes.size(); i++) delete this->nodeGenes[i];
	for(int i = 0; i < this->connectionGenes.size(); i++) delete this->connectionGenes[i];
}

int Genome::posOfRndCon(ConnectionDesc* cd){
	std::vector<int> enabledIndices;
	int conPos;

	for(int i = 0; i < this->connectionGenes.size(); i++){
		if(this->connectionGenes[i]->enabled){
			enabledIndices.push_back(i);
		}
	}

	this->gen.discard(10000);
	std::uniform_int_distribution<int> distr(0, enabledIndices.size() - 1);
	
	conPos = distr(this->gen);
	*cd = *(this->connectionGenes[conPos]);
	
	return conPos;
}

void Genome::rndCon(ConnectionDesc* cd){
	this->gen.discard(10000);

	int size = this->nodeGenes.size();
	std::vector<int> indexVec(size);
	for(int i = 0; i < size; i++) indexVec[i] = i;
	
	std::vector<int> orderedNodes = indexVec;
	std::vector<int> rndNodes(size);
	for(int i = (size-1); i >= 0; i--){
		std::uniform_int_distribution<int> distr(0, i);
		int pos = distr(this->gen);
		rndNodes[size - i - 1] = orderedNodes[pos];
		orderedNodes.erase(orderedNodes.begin()+pos);
	}

	for(int i = 0; i < size; i++){
		std::vector<bool> added(size, false);
		std::vector<int> remNodes;
		int id = this->nodeGenes[rndNodes[i]]->id;
		for(int j = 0; j < this->connectionGenes.size(); j++){
			int from = this->connectionGenes[j]->fromId;
			int to = this->connectionGenes[j]->toId;	
			if(!((from == id) or (to == id))){
				for(int k = 0; k < this->nodeGenes.size(); k++){
					if(this->nodeGenes[k]->id == from){
						if(!added[k]){
							remNodes.push_back(k);
							added[k] = true;
						}
					}else if(this->nodeGenes[k]->id == to){
						if(!added[k]){
							remNodes.push_back(k);
							added[k] = true;
						}
					}
				}
			}
		}

		if(remNodes.size() == 0) continue;
		
		cd->fromId = id;
		std::uniform_int_distribution<int> distr(0, remNodes.size() - 1);
		cd->toId = this->nodeGenes[remNodes[distr(this->gen)]]->id;
		return;
	}

	cd->fromId = -1;
	cd->toId = -1;
}

void Genome::mutateAddNode(int connectionToSplit, int innovNum){
	int from, to;
	float weight;

	from = this->connectionGenes[connectionToSplit]->fromId;
	to = this->connectionGenes[connectionToSplit]->toId;
	weight = this->connectionGenes[connectionToSplit]->weight;


	this->connectionGenes[connectionToSplit]->enabled = false;
	this->nodeGenes.push_back(new Node(Node::Type::HIDDEN, ++(this->highestNode)));
	this->connectionGenes.push_back(new Connection(from, this->highestNode, 1.0, true, innovNum));
	this->connectionGenes.push_back(new Connection(this->highestNode, to, weight, true, innovNum + 1));
}

void Genome::mutateAddConnection(ConnectionDesc* cd, int innovNum){
	this->gen.discard(10000);
	std::uniform_real_distribution<float> distr(-1.0, 1.0);
	
	this->connectionGenes.push_back(new Connection(cd->fromId, cd->toId, distr(this->gen), true, innovNum));
}

void Genome::stdMutateWeight(float min, float max){
	this->gen.discard(10000);
	std::uniform_int_distribution<int> distr1(0, this->connectionGenes.size() - 1);
	std::uniform_real_distribution<float> distr2(min, max);

	int mutCon = distr1(this->gen); 
	this->connectionGenes[mutCon]->weight *= distr2(this->gen);
	float tempWeight = this->connectionGenes[mutCon]->weight;  
	if(tempWeight < -1.0){
		this->connectionGenes[mutCon]->weight = -1.0;
	}else if(tempWeight > 1.0){
		this->connectionGenes[mutCon]->weight = 1.0;
	}
}

void Genome::rndMutateWeight(){
	this->gen.discard(10000);
	std::uniform_int_distribution<int> distr1(0, this->connectionGenes.size() - 1);
	std::uniform_real_distribution<float> distr2(-1.0, 1.0);
	
	this->connectionGenes[distr1(this->gen)]->weight = distr2(this->gen);
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
	std::vector<std::string> curLines(2, "");
	for(int i = 0; i < this->nodeGenes.size(); i++){
		curLines[0] = curLines[0] + alignLeft(unitWidth, "id: " + std::to_string(this->nodeGenes[i]->id));
		curLines[1] = curLines[1] + alignLeft(unitWidth, "type: " + type2Str(this->nodeGenes[i]->type));
	}
	std::cout << curLines[0] << std::endl << curLines[1] << std::endl;
	std::cout << std::string(maxWidth * unitWidth, '=') << std::endl << "Connections:" << std::endl;
	curLines.resize(5, "");
	curLines[0] = "";
	curLines[1] = "";
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

	


