#ifndef __NODE_H__
#define __NODE_H__

#include<vector>

class Node{
	public:
		enum Type{INPUT, HIDDEN, OUTPUT};
		Node(int pId, Node::Type pType, int pInnovNum);
		Node(int pId, int pInnovNum);
		~Node();

		int id;
		Node::Type type;
		float value;

		void load(std::vector<float> values, std::vector<float> weights);	

		int innovNum;

		void info();
};

#endif
