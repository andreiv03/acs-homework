package org.poo.cb.model;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.interfaces.Observer;
import org.poo.cb.serializers.CustomDoubleSerializer;
import org.poo.cb.strategies.CommissionFeeStrategy;
import org.poo.cb.strategies.PremiumCommissionFeeStrategy;
import org.poo.cb.strategies.StandardCommissionFeeStrategy;

import java.util.Arrays;
import java.util.HashMap;

public class Account implements Observer {
	private final CurrencyName currencyName;
	private double amount;
	private CommissionFeeStrategy commissionFeeStrategy;

	public Account(CurrencyName currencyName, double amount) {
		this.currencyName = currencyName;
		this.amount = amount;
		this.commissionFeeStrategy = new StandardCommissionFeeStrategy();
	}

	public CurrencyName getCurrencyName() {
		return currencyName;
	}

	@JsonSerialize(using = CustomDoubleSerializer.class)
	public double getAmount() {
		return amount;
	}

	public void withdraw(double amount) {
		this.amount = this.amount - amount;
	}

	public void deposit(double amount) {
		this.amount = this.amount + amount;
	}

	public double getExchangeRate(CurrencyName destinationCurrencyName) {
		HashMap<CurrencyName, Integer> currencyIndexMap = new HashMap<>();

		String[] exchangeRatesHeaderParts = Main.exchangeRates.get(0)
			.split(",");

		Arrays.stream(exchangeRatesHeaderParts)
			.skip(1)
			.forEach(currency -> currencyIndexMap.put(CurrencyName.valueOf(currency), currencyIndexMap.size() + 1));

		double exchangeRate = 1;

		for (String exchangeRates : Main.exchangeRates) {
			String[] exchangeRatesParts = exchangeRates.split(",");

			if (exchangeRatesParts[0].equals(this.currencyName.toString())) {
				int destinationCurrencyIndex = currencyIndexMap.get(destinationCurrencyName);
				exchangeRate = Double.parseDouble(exchangeRatesParts[destinationCurrencyIndex]);
				break;
			}
		}

		return exchangeRate;
	}

	public double getCommissionFee(double amount) {
		return commissionFeeStrategy.calculateCommissionFee(amount, this.amount);
	}

	public void setCommissionFeeStrategy(CommissionFeeStrategy commissionFeeStrategy) {
		this.commissionFeeStrategy = commissionFeeStrategy;
	}

	@Override
	public void update() {
		setCommissionFeeStrategy(new PremiumCommissionFeeStrategy());
	}
}
