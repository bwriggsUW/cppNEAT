#include "../headers/connection.h"
#include<iostream>

Connection::Connection(int pFromId, int pToId, float pWeight, bool pEnabled, int pInnovNum)
	: fromId(pFromId), toId(pToId), weight(pWeight), enabled(pEnabled), innovNum(pInnovNum){}
Connection::~Connection(){}

void Connection::info(){
	std::cout << "CONNECTION INFO:" << std::endl;
	std::cout << this->fromId << "->" << this->toId << std::endl;
	std::cout << "weight: " << this->weight << std::endl;
	std::cout << (this->enabled ? "ENABLED" : "DISABLED") << std::endl;
}
