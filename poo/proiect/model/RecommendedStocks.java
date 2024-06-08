package org.poo.cb.model;

import org.poo.cb.Main;

import java.util.ArrayList;

public class RecommendedStocks {
	public static volatile RecommendedStocks recommendedStocks = null;

	private final ArrayList<String> stocksToBuy = new ArrayList<>();

	private RecommendedStocks() {
		if (recommendedStocks != null)
			throw new RuntimeException("Use getInstance() method to get the single instance of this class!");
	}

	public static RecommendedStocks getInstance() {
		if (recommendedStocks == null) {
			synchronized (RecommendedStocks.class) {
				if (recommendedStocks == null)
					recommendedStocks = new RecommendedStocks();
			}
		}

		return recommendedStocks;
	}

	public ArrayList<String> getStocksToBuy() {
		return stocksToBuy;
	}

	public void resetStocksToBuy() {
		stocksToBuy.clear();
	}

	public void determineStocksToBuy() {
		for (String stockValues : Main.stockValues.subList(1, Main.stockValues.size())) {
			String[] stockValuesParts = stockValues.split(",");
			String stockName = stockValuesParts[0];

			ArrayList<Double> stockValuesList = new ArrayList<>();

			for (int index = stockValuesParts.length - 1; index >= 1; --index)
				stockValuesList.add(Double.parseDouble(stockValuesParts[index]));

			double shortTermSMA = calculateSMA(stockValuesList, 5);
			double longTermSMA = calculateSMA(stockValuesList, 10);

			if (shortTermSMA > longTermSMA)
				recommendedStocks.getStocksToBuy()
					.add(stockName);
		}
	}

	private double calculateSMA(ArrayList<Double> stockValuesList, int numberOfDays) {
		int endIndex = Math.min(numberOfDays, stockValuesList.size());
		double sum = 0;

		for (double stockValue : stockValuesList.subList(0, endIndex))
			sum = sum + stockValue;

		return sum / numberOfDays;
	}
}
