#ifndef __NODE_H__
#define __NODE_H__

#include<vector>

class Node{
	public:
		enum Type{INPUT, HIDDEN, OUTPUT};
		Node(Node::Type pType, int pId);
		~Node();

		int id;
		Node::Type type;
		float value;

		void load(std::vector<float> values, std::vector<float> weights);	

		void info();
};

#endif
