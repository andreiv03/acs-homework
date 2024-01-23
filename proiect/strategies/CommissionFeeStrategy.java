package org.poo.cb.strategies;

public abstract class CommissionFeeStrategy {
	public abstract double calculateCommissionFee(double amount, double accountAmount);
}
