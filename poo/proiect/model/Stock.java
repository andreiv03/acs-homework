package org.poo.cb.model;

import org.poo.cb.Main;

public class Stock {
	private final String stockName;
	private int amount;

	public Stock(String stockName, int amount) {
		this.stockName = stockName;
		this.amount = amount;
	}

	public String getStockName() {
		return stockName;
	}

	public int getAmount() {
		return amount;
	}

	public double findStockLatestValue() {
		double stockLatestValue = 0;

		for (String stockValues : Main.stockValues) {
			String[] stockValuesParts = stockValues.split(",");

			if (stockValuesParts[0].equals(this.stockName)) {
				String stockLatestValueString = stockValuesParts[stockValuesParts.length - 1];
				stockLatestValue = Double.parseDouble(stockLatestValueString);
				break;
			}
		}

		return stockLatestValue;
	}

	public void addStocks(int amount) {
		this.amount = this.amount + amount;
	}
}
