#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	size_t friends_count;
	size_t rules_count;
	vector<int> *adjacency_list_relations;
	vector<int> *adjacency_list_conflicts;

	void read_input() {
		ifstream fin("scandal.in");

		fin >> friends_count >> rules_count;
		adjacency_list_relations = new vector<int>[rules_count];
		adjacency_list_conflicts = new vector<int>[rules_count];

		for (size_t index = 0; index < rules_count; ++index) {
			int x, y, c;
			fin >> x >> y >> c;

			if (c == 1 || c == 3)
				y = y + friends_count;
			if (c == 2 || c == 3)
				x = x + friends_count;

			int x_complement = x > (int) friends_count ? x - friends_count
					: x + friends_count;
			int y_complement = y > (int) friends_count ? y - friends_count
					: y + friends_count;

			adjacency_list_relations[x].push_back(y_complement);
			adjacency_list_relations[y].push_back(x_complement);
			adjacency_list_conflicts[x_complement].push_back(y);
			adjacency_list_conflicts[y_complement].push_back(x);
		}

		fin.close();
	}

	void topological_sort(vector<int> &topological_order, vector<bool> &visited,
	                      int node) {
		visited[node] = true;

		for (const auto &neighbour : adjacency_list_conflicts[node])
			if (!visited[neighbour])
				topological_sort(topological_order, visited, neighbour);

		topological_order.push_back(node);
	}

	void search_component(vector<int> &components, size_t components_count,
												int node) {
		components[node] = components_count;

		for (const auto &neighbour : adjacency_list_relations[node])
			if (!components[neighbour])
				search_component(components, components_count, neighbour);
	}

	vector<int> get_result() {
		vector<int> topological_order;
		vector<bool> visited(2 * friends_count + 1, false);

		for (size_t index = 1; index <= 2 * friends_count; ++index)
			if (!visited[index])
				topological_sort(topological_order, visited, index);

		vector<int> components(2 * friends_count + 1, 0);
		size_t component_count = 0;

		for (auto node = topological_order.rbegin();
			node != topological_order.rend(); ++node)
			if (!components[*node])
				search_component(components, ++component_count, *node);

		vector<int> results;

		for (size_t index = 1; index <= friends_count; ++index)
			if (components[index] > components[index + friends_count])
				results.push_back(index);

		delete[] adjacency_list_relations;
		delete[] adjacency_list_conflicts;

		return results;
	}

	void print_output(vector<int> results) {
		ofstream fout("scandal.out");
		fout << results.size() << '\n';
		for (const auto &result : results)
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
