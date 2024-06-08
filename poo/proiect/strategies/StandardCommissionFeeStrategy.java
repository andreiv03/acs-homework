package org.poo.cb.strategies;

public class StandardCommissionFeeStrategy extends CommissionFeeStrategy {
	@Override
	public double calculateCommissionFee(double amount, double accountAmount) {
		return amount > accountAmount / 2 ? amount * 0.01 : 0;
	}
}
