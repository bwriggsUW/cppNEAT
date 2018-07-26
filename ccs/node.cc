#include "../headers/node.h"
#include<iostream>
#include<vector>

Node::Node(int pId, Node::Type pType, int pInnovNum) : id(pId), type(pType), innovNum(pInnovNum) ,value(0.0){}

Node::Node(int pId, int pInnovNum) : Node::Node(pId, Node::Type::HIDDEN, pInnovNum){}

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
}
