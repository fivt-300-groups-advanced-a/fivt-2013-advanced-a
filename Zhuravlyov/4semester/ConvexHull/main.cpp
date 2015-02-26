#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

struct Vertex {
    int x;
    int y;

	Vertex(int _x, int _y) {
		x = _x;
		y = _y;
	}

    Vertex operator-(const Vertex& another) {
        return Vertex(x - another.x, y - another.y);
    }

	bool operator<(const Vertex& another) {
		return x < another.x || x == another.x && y < another.y;
	}
};

struct Vertex_comparator {
    bool operator()(const Vertex& first, const Vertex& second) {
        return first.x < second.y || first.x == second.x && first.y < second.y;
    }
};

int vector_product(Vertex a, Vertex b) {
    return a.x * b.y - a.y * b.x;
}

int rotation(Vertex a, Vertex b, Vertex c) {
    return vector_product(b - a, c - b);
}

long double half_convex_hull(const std::vector<Vertex>& source) {
	std::vector<Vertex> convex_hull;
	convex_hull.push_back(source[0]);
	convex_hull.push_back(source[1]);
	for (unsigned int i = 2; i < source.size(); i++) {
		while (convex_hull.size() > 1 && rotation(*(convex_hull.end() - 1), *(convex_hull.end() - 2), source[i]) <= 0) {
			convex_hull.pop_back();
		}
		convex_hull.push_back(source[i]);
    }
	long double ans = 0;
	for (unsigned int i = 1; i < convex_hull.size(); i++) {
		Vertex segment = convex_hull[i] - convex_hull[i - 1];
		ans += sqrtl(segment.x * segment.x + segment.y * segment.y);
	}
	return ans;
}

long double convex_hull_length(std::vector<Vertex>& verteces) {
    std::sort(verteces.begin(), verteces.end());
    Vertex left = verteces[0];
    Vertex right = verteces.back();
    std::vector<Vertex> up_half, down_half;
    for (unsigned int i = 0; i < verteces.size(); i++) {
        int rot = rotation(left, right, verteces[i]);
        if (rot >= 0)
            up_half.push_back(verteces[i]);
        if (rot <= 0)
            down_half.push_back(verteces[i]);
    }
    std::reverse(down_half.begin(), down_half.end());
	return half_convex_hull(up_half) + half_convex_hull(down_half);
}

int main() {
	unsigned int n, l;
	std::cin >> n >> l;
	std::vector<Vertex> points;
	for (unsigned int i = 0; i < n; i++) {
		int x, y;
		std::cin >> x >> y;
		points.push_back(Vertex(x, y));
	}
	long double pi = 3.14159265358979323846;
	long double ans = convex_hull_length(points) + 2 * pi * l;
	std::cout << (int)(ans + 0.5);
    return 0;
}