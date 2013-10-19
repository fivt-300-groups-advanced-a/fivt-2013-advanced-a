#include <iostream>
#include <fstream>
#include "binary_file_io.h"
#include "sort.h"
#include "comparator.h"
#include "istream_io.h"
using namespace std;

int main() {
    ofstream of("output.txt");
    bigSort<int>(new ISReader<int>(new ifstream("input.txt")), 
                 new OSWriter<int>(&of, " "));
    of.close();
}
