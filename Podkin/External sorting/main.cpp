#include "FileWriter.h"
#include "FileReader.h"
#include "StandartSorter.h"
#include "ExternalSorter.h"
#include "functional"
#include <iostream>

int main(){
    srand(time(NULL));
    FileWriter<int> out("input.txt");
    int NumberCounter[(1 << 20)];
    for (int i = 0; i < (1 << 20); i++)
        NumberCounter[i] = 0;
    for (int i = 0; i < (1 << 20); i++){
        int Number = rand() % (1 << 20);
        NumberCounter[Number]++;
        if (!out(Number)){
            std::cout << "1\n";
            break;
        }
    }
    out.SetStream("output.txt");
    FileReader<int> in("input.txt");
    StandartSorter<int> sort;
    ExternalSorter<int> ext_sort;
    std::less<int> comp;
    ext_sort(1 << 10, in, out, sort, comp);
    int Numbers[(1 << 20)];
    for (int i = 0; i < (1 << 20); i++)
        Numbers[i] = 0;
    in.SetStream("output.txt");
    int Number;
    if (!in(Number)){
        std::cout << "2\n";
    }
    Numbers[Number]++;
    for (int i = 1; i < (1 << 20); i++){
        int SecondNumber;
        if (!in(SecondNumber)){
        std::cout << "2\n";
        }
        if (Number > SecondNumber){            
        std::cout << "3\n";
        }
        Number = SecondNumber;
        Numbers[Number]++;
    }
    for (int i = 0; i < (1 << 20); i++){
        if (NumberCounter[i] != Numbers[i]){
        std::cout << "4\n";
        }
    }
}
