#include "filesort.h"
#include <iostream>

using namespace std;

int main() 
{
	//freopen("input.txt", "w", stdout);
	//for(int i = 0; i < 1000; i++) {
	//	cout << rand() % 1000000 << endl;
	//}
	filesort<int, less<int> >("input.txt", "output.txt", 4000);
}