#include<iostream>
#include<limits>
using namespace std;

int main(){

	const int max_int = std::numeric_limits<int>::max();
	const long int max_long = std::numeric_limits<long int>::max();
	const long long int max_long_long = std::numeric_limits<long long int>::max();

	std::cout << "maximum int " << max_int << endl;
	std::cout << "maximum long int " << max_long << endl;
	std::cout << "maximum long long int " << max_long_long << endl << endl;

	std::cout << "size of int " << sizeof(int) << endl;
	std::cout << "size of long int " << sizeof(long int) << endl;
	std::cout << "size of long long int " << sizeof(long long int) << endl;

	return 0;

}
