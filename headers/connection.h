#ifndef __CONNECTION_H__
#define __CONNECTION_H__

class Connection{
	public:
		Connection(int pFromId, int pToId, float pWeight, bool pEnabled, int pInnovNum);
		~Connection();

		int fromId;
		int toId;
		float weight;
		bool enabled;

		void info();

		int innovNum;
};

#endif
