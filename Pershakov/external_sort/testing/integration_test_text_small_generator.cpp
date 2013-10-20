#include <fstream>
#include <ctime>
#include <cstdlib>

long long SMALL_TEST_SIZE = 500;

int main(int argc, char *argv[]){
    srand(time(0));
    std::ofstream out("tests/integration_test_text_small.in");
    if (argc > 1)
        SMALL_TEST_SIZE = atoll(argv[1]);
    for (int i = 0; i < SMALL_TEST_SIZE; i++)
        out << rand() % 1000000 << ' ';
    out << "\n";
}
