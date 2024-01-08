int getHighestPowerOfTwo(int number) {
	int power = 1;

	while (power <= number)
		power <<= 1;

	power >>= 1;
	return power;
}