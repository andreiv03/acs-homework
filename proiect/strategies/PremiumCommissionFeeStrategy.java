package org.poo.cb.strategies;

public class PremiumCommissionFeeStrategy extends CommissionFeeStrategy {
	@Override
	public double calculateCommissionFee(double amount, double accountAmount) {
		return 0;
	}
}
