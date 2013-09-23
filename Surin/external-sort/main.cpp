#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <memory>
#include <thread>
#include "binary_file_io.h"
#include "sort.h"
#include "comparator.h"
using namespace std;

#define INF 1e+9



int main() {
    //ios_base::sync_with_stdio(false);
/*    BFWriter<int> sw("temp");
    sw << 5 << 6;
    sw.close();
    BFReader<int> ss("temp");
    int x, y; 
    ss >> x; 
    cout << ss.eos();
    ss >> y;
    cout << ss.eos();
    int zz;
    ss >> zz;
    cout << ss.eos();
    cout << "\n" << x << " " << y;*/
 //   cout <<  CPair<int, int, CLess<int>, CInvOp<int, CLess<int> > > ()(make_pair(1, 2), make_pair(1, 3));
    bigSort(new BFReader<int>("file1"), new BFWriter<int>("file2"), new CLess<int>());
}
