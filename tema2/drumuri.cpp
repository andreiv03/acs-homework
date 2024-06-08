#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	const long long INF = 1e18;

	size_t nodes_count;
	size_t edges_count;
	vector <pair<int, int>> *adjacency_list_forward;
	vector <pair<int, int>> *adjacency_list_backward;
	int node_x, node_y, node_z;

	void read_input() {
		ifstream fin("drumuri.in");

		fin >> nodes_count >> edges_count;
		adjacency_list_forward = new vector <pair<int, int>>[nodes_count + 1];
		adjacency_list_backward = new vector <pair<int, int>>[nodes_count + 1];

		for (size_t index = 0; index < edges_count; ++index) {
			int node_one, node_two, cost;
			fin >> node_one >> node_two >> cost;
			adjacency_list_forward[node_one].push_back({node_two, cost});
			adjacency_list_backward[node_two].push_back({node_one, cost});
		}

		fin >> node_x >> node_y >> node_z;
		fin.close();
	}

	void dijkstra(vector <pair<int, int>> *adjacency_list,
								vector<long long> &distances, int start_node) {
		priority_queue <pair<long long, int>, vector <pair<long long, int>>,
			greater <pair<long long, int>>> PQ;
		distances[start_node] = 0;
		PQ.push({distances[start_node], start_node});

		while (!PQ.empty()) {
			int node = PQ.top().second;
			long long cost = PQ.top().first;
			PQ.pop();

			if (cost <= distances[node]) {
				for (const auto &neighbour : adjacency_list[node]) {
					long long distance = distances[node] + neighbour.second;

					if (distance < distances[neighbour.first]) {
						distances[neighbour.first] = distance;
						PQ.push({distance, neighbour.first});
					}
				}
			}
		}
	}

	long long get_result() {
		long long result = INF;

		vector<long long> distances_from_x(nodes_count + 1, INF);
		vector<long long> distances_from_y(nodes_count + 1, INF);
		vector<long long> distances_to_z(nodes_count + 1, INF);

		dijkstra(adjacency_list_forward, distances_from_x, node_x);
		dijkstra(adjacency_list_forward, distances_from_y, node_y);
		dijkstra(adjacency_list_backward, distances_to_z, node_z);

		for (size_t node = 1; node <= nodes_count; ++node)
			result = min(result, distances_from_x[node] + distances_from_y[node]
				+ distances_to_z[node]);

		delete[] adjacency_list_forward;
		delete[] adjacency_list_backward;

		if (result == INF)
			return -1;

		return result;
	}

	void print_output(long long result) {
		ofstream fout("drumuri.out");
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
