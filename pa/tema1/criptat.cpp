#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	size_t words_count;
	vector <string> words;

	void read_input() {
		ifstream fin("criptat.in");

		fin >> words_count;
		words.resize(words_count);

		for (string &word : words)
			fin >> word;

		fin.close();
	}

	static bool comparator(const string &a, const string &b,
												 char current_character) {
		double ratio_string_a = 0.0;
		double ratio_string_b = 0.0;

		for (char character : a)
			if (character == current_character)
				ratio_string_a = ratio_string_a + 1;

		for (char character : b)
			if (character == current_character)
				ratio_string_b = ratio_string_b + 1;

		ratio_string_a = ratio_string_a / a.size();
		ratio_string_b = ratio_string_b / b.size();

		return ratio_string_a > ratio_string_b;
	}

	int get_result() {
		vector<int> lengths;
		lengths.resize(26);

		for (char character = 'a'; character <= 'z'; ++character) {
			int current_length = 0;
			int character_apparitions = 0;

			sort(words.begin(), words.end(), bind(comparator, placeholders::_1,
																						placeholders::_2, character));

			for (auto &word : words) {
				int character_count = 0;

				for (char character_word : word)
					if (character_word == character)
						character_count = character_count + 1;

				if ((character_apparitions + character_count) * 2 >
						current_length + word.size()) {
					current_length = current_length + word.size();
					character_apparitions = character_apparitions + character_count;
				}
			}

			lengths[character - 'a'] = current_length;
		}

		int maximum_length = 0;

		for (int length : lengths)
			if (length > maximum_length)
				maximum_length = length;

		return maximum_length;
	}

	void print_output(int result) {
		ofstream fout("criptat.out");
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
