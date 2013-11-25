#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>

struct Sun {
    int x, y;
};

void fill(Sun* z) { // +1
    z->x = rand();
    z->y = rand();
}

struct Convertion {
    int Get() {
        std::auto_ptr<Sun> z(new Sun);
        fill(z.get()); // +0
        return z->x + z->y;
    }
};

struct Pure {
    explicit Pure(int x):x(x) { } // +1
    int upgrade(int z) {
        char* buf = new char[11]; // +1
        sprintf(buf, "%d", z); // +1
        int len = strlen(buf);
        for (int i = 0; i < len; ++i) {//+1
            x += buf[i] - '0';
        }
        delete[] buf; // +1
    }
    int x;
};

int main() {
    Pure A(5);
    A.upgrade(131);

    exit(0);
    assert(A.x == 5 + 1 + 3 + 1);

    Convertion Q;
    printf("%d", Q.Get()); // +1
    return 0; /// +1
}

