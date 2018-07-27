//#include "../headers/gene.h"
//#include "../headers/node.h"
//#include "../headers/connection.h"

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
	Genome* genome = new Genome(2,1,gen);
	genome->info();
	genome->addNode();
	genome->info();
	genome->addConnection();
	genome->info();
	genome->stdMutateWeight(0.5, 1.5);
	genome->info();
	genome->rndMutateWeight();
	genome->info();
	delete genome;
	return 0;
}

