#include "../headers/gene.h"
#include<iostream>

int Gene::innovations = 0;

Gene::Gene() : mInnovNum(Gene::innovations++){}

Gene::~Gene(){}

int Gene::innovNum(){
	return this->mInnovNum;
}

