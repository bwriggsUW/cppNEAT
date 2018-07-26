#include<iostream>
#include<vector>
//#include "../headers/gene.h"
//#include "../headers/node.h"
//#include "../headers/connection.h"
#include "../headers/genome.h"

int main(int argc, char* argv[]){
	Genome* genome = new Genome(2,1);
	genome->info();
	genome->addNode();
	genome->info();
	genome->addConnection();
	genome->info();
	delete genome;
	return 0;
}

