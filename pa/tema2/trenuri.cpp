#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	string source, destination;
	size_t edges_count;
	vector<int> *adjacency_list;
	unordered_map<string, int> cities;

	void read_input() {
		ifstream fin("trenuri.in");

		fin >> source >> destination >> edges_count;
		adjacency_list = new vector<int>[edges_count];

		for (size_t index = 0; index < edges_count; ++index) {
			string city_one, city_two;
			fin >> city_one >> city_two;

			if (cities.find(city_one) == cities.end())
				cities[city_one] = cities.size();

			if (cities.find(city_two) == cities.end())
				cities[city_two] = cities.size();

			adjacency_list[cities[city_one]].push_back(cities[city_two]);
		}

		fin.close();
	}

	void topological_sort(vector<int> &topological_order, vector<bool> &visited,
												int node) {
		visited[node] = true;

		for (const auto &neighbour : adjacency_list[node])
			if (!visited[neighbour])
				topological_sort(topological_order, visited, neighbour);

		topological_order.insert(topological_order.begin(), node);
	}

	int get_result() {
		vector<int> topological_order;
		vector<bool> visited(cities.size(), false);
		topological_sort(topological_order, visited, cities[source]);

		vector<int> distances(cities.size(), 0);
		distances[cities[source]] = 1;

		for (const auto &node : topological_order)
			for (const auto &neighbour : adjacency_list[node])
				if (distances[node] > 0)
					distances[neighbour] = max(distances[neighbour], distances[node] + 1);

		delete[] adjacency_list;
		return distances[cities[destination]];
	}

	void print_output(int result) {
		ofstream fout("trenuri.out");
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
