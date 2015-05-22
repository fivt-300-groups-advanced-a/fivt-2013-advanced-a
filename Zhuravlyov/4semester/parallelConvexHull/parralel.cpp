#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <thread>
#include <future>
#include <chrono>

typedef long long numberType;

struct Vertex {
	numberType x;
	numberType y;

	Vertex(numberType _x, numberType _y) {
		x = _x;
		y = _y;
	}

	Vertex operator-(const Vertex& another) {
		return Vertex(x - another.x, y - another.y);
	}

	bool operator<(const Vertex& another) const {
		return std::make_pair(x, y) < std::make_pair(another.x, another.y);
	}
};

numberType vector_product(Vertex a, Vertex b) {
	return a.x * b.y - a.y * b.x;
}

numberType rotation(Vertex a, Vertex b, Vertex c) {
	return vector_product(b - a, c - b);
}

std::vector<Vertex> half_convex_hull(const std::vector<Vertex>& source) {
	std::vector<Vertex> convex_hull;
	convex_hull.push_back(source[0]);
	convex_hull.push_back(source[1]);
	for (unsigned int i = 2; i < source.size(); i++) {
		while (convex_hull.size() > 1 && rotation(*(convex_hull.end() - 2), *(convex_hull.end() - 1), source[i]) >= 0) {
			convex_hull.pop_back();
		}
		convex_hull.push_back(source[i]);
	}
	return std::move(convex_hull);
}


void parallel_sort(std::vector<Vertex>& vector, int left, int right, int number_of_avaliable_threads) {
	if (left == right) {
		return;
	} else if (number_of_avaliable_threads == 1) {
		std::sort(vector.begin() + left, vector.begin() + right + 1);
	} else {
		int mid = (left + right) / 2;
		int left_threads_number = number_of_avaliable_threads / 2;
		int right_threads_number = number_of_avaliable_threads - left_threads_number;
		std::thread first(parallel_sort, std::ref(vector), left, mid, left_threads_number);
		std::thread second(parallel_sort, std::ref(vector), mid + 1, right, right_threads_number);
		first.join();
		second.join(); 
		std::vector<Vertex> res;
		int left_part = left;
		int rigth_part = mid + 1;
		while (left_part <= mid || rigth_part <= right) {
			if (rigth_part > right || left_part <= mid && vector[left_part] < vector[rigth_part]) {
				res.push_back(vector[left_part]);
				left_part++;
			}
			else {
				res.push_back(vector[rigth_part]);
				rigth_part++;
			}
		}
		for (int i = 0; i < res.size(); i++) {
			vector[left + i] = res[i];
		}
	}
}

std::vector<Vertex> get_convex_hull(std::vector<Vertex>& verteces) {
	parallel_sort(verteces, 0, verteces.size() - 1, std::thread::hardware_concurrency());
	Vertex left = verteces[0];
	Vertex right = verteces.back();
	std::vector<Vertex> up_half, down_half;
	for (unsigned int i = 0; i < verteces.size(); i++) {
		numberType rot = rotation(left, right, verteces[i]);
		if (rot >= 0)
			up_half.push_back(verteces[i]);
		if (rot <= 0)
			down_half.push_back(verteces[i]);
	}
	std::reverse(down_half.begin(), down_half.end());
	std::future< std::vector<Vertex> > upper_part_fut = std::async(std::launch::async, half_convex_hull, up_half);
	std::future< std::vector<Vertex> > downward_part_fut = std::async(std::launch::async, half_convex_hull, down_half);
	std::vector<Vertex> upper_part = upper_part_fut.get();
	std::vector<Vertex> downward_part = downward_part_fut.get();
	for (int i = 1; i < downward_part.size() - 1; i++)
		upper_part.push_back(downward_part[i]);
	return std::move(upper_part);
}

using namespace std::chrono;

int main(int argc, char** argv) {
	unsigned int n;
	std::cin >> n;
	std::vector<Vertex> points;
	for (unsigned int i = 0; i < n; i++) {
		long long x, y;
		std::cin >> x >> y;
		points.push_back(Vertex(x, y));
	}
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	std::vector<Vertex> convex = get_convex_hull(points);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	if (argc == 1) {
		std::cout << convex.size() << std::endl;
		for (int i = 0; i < convex.size(); i++) {
			std::cout << convex[i].x << " " << convex[i].y << std::endl;
		}
	}
	else {
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		std::cout << "Calculation time in microseconds: " << duration << std::endl;
	}
	return 0;
}