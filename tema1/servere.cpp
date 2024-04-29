#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	size_t servers_count;
	vector <pair<int, int>> servers;

	void read_input() {
		ifstream fin("servere.in");

		fin >> servers_count;
		servers.resize(servers_count);

		for (auto &server : servers)
			fin >> server.first;

		for (auto &server : servers)
			fin >> server.second;

		fin.close();
	}

	double get_result() {
		const double EPSILON = 0.1;
		double result = INT_MIN;

		double left = min_element(servers.begin(), servers.end(),
		                          [](const pair<int, int> &a,
		                             const pair<int, int> &b) {
			                          return a.second < b.second;
		                          })->second;

		double right = max_element(servers.begin(), servers.end(),
		                           [](const pair<int, int> &a,
		                              const pair<int, int> &b) {
			                           return a.second < b.second;
		                           })->second;

		while (right - left > EPSILON) {
			double middle = (left + right) / 2;
			double left_value = INT_MAX;
			double value = INT_MAX;
			double right_value = INT_MAX;

			for (auto &server : servers) {
				left_value = min(left_value, server.first - abs(middle - EPSILON -
				                                                server.second));
				value = min(value, server.first - abs(middle - server.second));
				right_value = min(right_value, server.first - abs(middle + EPSILON -
				                                                  server.second));
			}

			if (value > result)
				result = value;

			if (left_value <= value)
				left = middle;
			else
				right = middle;
		}

		return result;
	}

	void print_output(double result) {
		ofstream fout("servere.out");
		fout << fixed << setprecision(1) << result << '\n';
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
