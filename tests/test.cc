//#include "../headers/node.h"
#include "../headers/connection.h"

#include<iostream>
#include<vector>
#include<random>
#include<fstream>
#include "../headers/genome.h"

unsigned long long int getSeed(){
	unsigned long long int randomValue = 0;
	size_t size = sizeof(randomValue);
	std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
	if(urandom){
		urandom.read(reinterpret_cast<char*>(&randomValue), size);
		if(!urandom) std::cerr << "Failed to read from /dev/urandom" << std::endl;
		urandom.close();
	}else std::cerr << "Failed to open /dev/urandom" << std::endl;
	return randomValue;
}

int main(int argc, char* argv[]){
	std::default_random_engine gen(getSeed());
	
	Genome* genome1 = new Genome(2,1,gen);
	Genome* genome2 = new Genome(2,1,gen);

	std::cout << "genome1:" << std::endl;
	genome1->info();
	std::cout << std::endl;
	std::cout << "genome2:" << std::endl;
	genome2->info();
	std::cout << std::endl;

	ConnectionDesc* cd = new ConnectionDesc;
	int connectionToSplit = genome1->posOfRndCon(cd);
	genome1->mutateAddNode(connectionToSplit, 2);
	connectionToSplit = genome2->posOfRndCon(cd);
	genome2->mutateAddNode(connectionToSplit, 4);

	std::cout << "genome1 + one node:" << std::endl;
	genome1->info();
	std::cout << std::endl;
	std::cout << "genome2 + one node:" << std::endl;
	genome2->info();
	std::cout << std::endl;

	genome1->rndCon(cd);
	if((cd->fromId >= 0) and (cd->toId >= 0)){
		genome1->mutateAddConnection(cd, 6);
	}else{
		std::cout << "already fully connected" << std::endl;
	}
	genome2->rndCon(cd);
	if((cd->fromId >= 0) and (cd->toId >= 0)){
		genome2->mutateAddConnection(cd, 7);
	}else{
		std::cout << "already fully connected" << std::endl;
	}

	std::cout << "genome1 + one node + one connection:" << std::endl;
	genome1->info();
	std::cout << std::endl;
	std::cout << "genome2 + one node + one connection:" << std::endl;
	genome2->info();
	std::cout << std::endl;

	genome1->rndMutateWeight();
	genome2->stdMutateWeight(0.5,1.5);

	std::cout << "genome1 + one node + one connection + one random weight mutation:" << std::endl;
	genome1->info();
	std::cout << std::endl;
	std::cout << "genome2 + one node + one connection + one standard weight mutation:" << std::endl;
	genome2->info();
	std::cout << std::endl;

	std::cout << Genome::compatibility(*genome1, *genome2, 0.5, 0.5, 0.5) << std::endl;

	delete genome1;
	delete genome2;
	delete cd;
	
	return 0;
}

