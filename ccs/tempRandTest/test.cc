#include<iostream>
#include<random>
#include<algorithm>
#include<functional>

//int main(int argc, char* argv[]){
//	std::vector<bool> enabled {true, false, true, true, false, false};
//	std::random_device rd;
//	std::default_random_engine generator(rd());
//	int index = -1;
//	for(int i = 0, j = enabled.size(); i < enabled.size(); i++, j--){
//		std::uniform_int_distribution<int> distribution(1, j);
//		if(enabled[i]){
//			auto myRnd = std::bind(distribution, generator);
//			if(myRnd() == myRnd()){
//				index = i;
//				break;
//			}
//		}
//	}
//	std::cout << index << std::endl;
//	return 0;
//
//}

int main(int argc, char* argv[]){
	std::vector<bool> enabled {true, false, true, true, false, false};
	std::vector<int> indices;
	for(int i = 0; i < enabled.size(); i++) if(enabled[i]) indices.push_back(i);
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<int> distr(0, indices.size() - 1);
	
	int n = 10000;
	std::vector<int> results(indices.size());
	for(int i = 0; i < n; i++) results[distr(gen)]++;

	for(int i = 0; i < indices.size(); i++){
		std::cout << indices[i] << ": " << (float)results[i]/n << std::endl;
	}
	return 0;
}





	

