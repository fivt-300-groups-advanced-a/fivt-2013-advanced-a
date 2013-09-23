#include "input/text_file_reader.h"
#include "input/binary_file_reader.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    cout << "Testing binary_reader:\n";
    ifstream bin("bin.txt", ifstream::binary | ifstream::in);
    Binary_file_reader<int> breader(bin);
    int a, b, c;
    if (!breader(a))
        cout << "BBAD1" << endl;
    cout << a << endl;
    if (!breader(b))
        cout << "BBAD2" << endl;
    cout << b << endl;;
    if (!breader(c))
        cout << "BBAD3" << endl;
    cout << c << endl;


    cout << "Testing reader\n";
    ifstream in("test.txt", ifstream::in);

    Text_file_reader<int> reader("test.txt");
    int k;
    reader(k);
    cout << k << endl;
    if (!reader(k))
        cout << "BAD" << endl;
    cout << k << endl;
    if (!reader(k))
        cout << "BAD" << endl;
    cout << k << endl;
}
