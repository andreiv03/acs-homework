#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	size_t areas_count;
	vector <pair<long long, char>> areas;

	void read_input() {
		ifstream fin("colorare.in");

		fin >> areas_count;
		areas.resize(areas_count);

		for (auto &area : areas)
			fin >> area.first >> area.second;

		fin.close();
	}

	long long pow_modulo(long long base, unsigned int exponent, const int modulo) {
		long long result = 1;

		while (exponent > 0) {
			if (exponent & 1)
				result = result * base % modulo;

			exponent = exponent >> 1;
			base = base * base % modulo;
		}

		return result;
	}

	long long get_result() {
		const int MODULO = 1e9 + 7;

		char orientation = areas[0].second;
		long long result = orientation == 'V' ? 3 : 6;

		if (areas[0].first - 1 > 0)
			result = result * pow_modulo(orientation == 'V' ? 2 : 3,
																	 areas[0].first - 1, MODULO);

		for (auto &area : areas) {
			if (&area == &areas[0])
				continue;

			if (area.second == 'V')
				result = result * (orientation == 'H' ? 1 : 2);

			if (area.second == 'H')
				result = result * (orientation == 'V' ? 2 : 3);

			area.first = area.first - 1;

			if (area.first > 0)
				result = result * pow_modulo(area.second == 'V' ? 2 : 3,
																		 area.first, MODULO) % MODULO;

			orientation = area.second;
		}

		return result;
	}

	void print_output(long long result) {
		ofstream fout("colorare.out");
		fout << result << '\n';
		fout.close();
	}
};

int main() {
	auto *task = new(nothrow) Task();

	if (!task) {
		cerr << "Something went wrong!\n";
		return -1;
	}

	task->solve();
	delete task;

	return 0;
}
