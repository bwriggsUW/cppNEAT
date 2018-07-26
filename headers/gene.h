#ifndef __GENE_H__
#define __GENE_H__

class Gene{
	public:
		Gene();
		~Gene();
		int innovNum();
		virtual void info() = 0;
	private:
		int mInnovNum;
		static int innovations;
};

#endif
