#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	const int MODULO = 1e9 + 7;

	size_t nodes_count;
	size_t edges_count;
	vector<int> *adjacency_list_one;
	vector<int> *adjacency_list_two;

	void read_input() {
		ifstream fin("numarare.in");

		fin >> nodes_count >> edges_count;
		adjacency_list_one = new vector<int>[nodes_count + 1];
		adjacency_list_two = new vector<int>[nodes_count + 1];

		for (size_t index = 0; index < edges_count; ++index) {
			int node_one, node_two;
			fin >> node_one >> node_two;
			adjacency_list_one[node_one].push_back(node_two);
		}

		for (size_t index = 0; index < edges_count; ++index) {
			int node_one, node_two;
			fin >> node_one >> node_two;
			adjacency_list_two[node_one].push_back(node_two);
		}

		fin.close();
	}

	int get_result() {
		vector<int> *intersection = new vector<int>[nodes_count + 1];

		for (size_t node = 1; node <= nodes_count; ++node)
			for (const auto &neighbour_one : adjacency_list_one[node])
				for (const auto &neighbour_two : adjacency_list_two[node])
					if (neighbour_one == neighbour_two)
						intersection[node].push_back(neighbour_one);

		vector<int> dp(nodes_count + 1, 0);
		dp[1] = 1;

		for (size_t node = 1; node <= nodes_count; ++node)
			if (dp[node] != 0)
				for (const auto &neighbour : intersection[node])
					dp[neighbour] = (dp[neighbour] + dp[node]) % MODULO;

		delete[] adjacency_list_one;
		delete[] adjacency_list_two;
		delete[] intersection;

		return dp[nodes_count];
	}

	void print_output(int result) {
		ofstream fout("numarare.out");
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
