#include <fstream>
#include <ctime>
#include <cstdlib>

long long BIG_TEST_SIZE = 4000000;

int main(int argc, char *argv[]){
    srand(time(0));
    std::ofstream out("tests/integration_test_text_big.in");
    if (argc > 1)
        BIG_TEST_SIZE = atoll(argv[1]);
    for (int i = 0; i < BIG_TEST_SIZE; i++)
        out << rand() % 1000000 << ' ';
    out << "\n";
}
