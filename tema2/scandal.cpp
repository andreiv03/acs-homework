#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	void read_input() {
		ifstream fin("scandal.in");
		fin.close();
	}

	int get_result() {
		return -1;
	}

	void print_output(int result) {
		ofstream fout("scandal.out");
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
