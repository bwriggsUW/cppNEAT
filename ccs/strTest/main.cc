#include<iostream>
#include<string>

int main(int argc, char* argv[]){
	std::string str = "test";
	std::cout << str + std::to_string(1.0) << std::endl;
	return 0;
}
