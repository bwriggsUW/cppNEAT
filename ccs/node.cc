#include "../headers/node.h"
#include<iostream>
#include<vector>

Node::Node(Node::Type pType, int pId) : id(pId), type(pType), value(0.0){}

Node::~Node(){}

void Node::load(std::vector<float> values, std::vector<float> weights){
	if(this->type == Node::Type::INPUT) return;
	this->value = 0.0;
	for(int i = 0; i < values.size(); i++){
		this->value += (values[i]*weights[i]);
	}
}

void Node::info(){
	std::cout << "NODE INFO:" << std::endl;
	std::cout << "id: " << this->id << std::endl;
	std::cout << "type: " << this->type << std::endl;
	std::cout << "value: " << this->value << std::endl;
}
