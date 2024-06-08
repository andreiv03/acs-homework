#include <bits/stdc++.h>

using namespace std;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int products_count, k;
	vector<int> prices;

	void read_input() {
		ifstream fin("oferta.in");

		fin >> products_count >> k;
		prices.resize(products_count);

		for (int &price : prices)
			fin >> price;

		fin.close();
	}

	double get_result() {
		vector<double> DP;
		DP.resize(products_count + 1);

		DP[0] = 0.0;

		for (int index = 1; index <= products_count; ++index) {
			double smaller_price = min(prices[index - 2], prices[index - 1]);
			double bigger_price = max(prices[index - 2], prices[index - 1]);
			DP[index] = min(DP[index - 1] + prices[index - 1], DP[index - 2]
				+ smaller_price / 2.0 + bigger_price);

			int price_first_previous = prices[index - 1];
			int price_second_previous = prices[index - 2];
			int price_third_previous = prices[index - 3];

			if (price_third_previous > price_second_previous)
				swap(price_third_previous, price_second_previous);

			if (price_second_previous > price_first_previous)
				swap(price_second_previous, price_first_previous);

			if (price_third_previous > price_second_previous)
				swap(price_third_previous, price_second_previous);

			DP[index] = min(DP[index], DP[index - 3] + price_first_previous
				+ price_second_previous);
		}

		return DP[products_count];
	}

	void print_output(double result) {
		ofstream fout("oferta.out");
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
