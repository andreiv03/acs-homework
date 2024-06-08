#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	size_t n, m;
	vector<int> A, B;

	void read_input() {
		ifstream fin("compresie.in");

		fin >> n;
		A.resize(n);

		for (auto &a : A)
			fin >> a;

		fin >> m;
		B.resize(m);

		for (auto &b : B)
			fin >> b;

		fin.close();
	}

	int get_result() {
		int result = 0;
		int A_sum = accumulate(A.begin(), A.end(), 0);
		int B_sum = accumulate(B.begin(), B.end(), 0);

		if (A_sum != B_sum)
			return -1;

		size_t A_index = 0;
		size_t B_index = 0;
		A_sum = 0;
		B_sum = 0;

		while (A_index < n && B_index < m) {
			if (A_sum == 0 && B_sum == 0) {
				A_sum = A[A_index];
				B_sum = B[B_index];
			}

			if (A_sum < B_sum)
				A_sum = A_sum + A[++A_index];

			if (A_sum > B_sum)
				B_sum = B_sum + B[++B_index];

			if (A_sum == B_sum) {
				result = result + 1;
				A_sum = 0;
				B_sum = 0;
				A_index = A_index + 1;
				B_index = B_index + 1;
			}
		}

		return result;
	}

	void print_output(int result) {
		ofstream fout("compresie.out");
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
