package org.poo.cb.model;

import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.interfaces.Observer;
import org.poo.cb.interfaces.Subject;

import java.util.ArrayList;

public class Portfolio implements Subject {
	private final ArrayList<Stock> stocks;
	private final ArrayList<Account> accounts;
	private final ArrayList<Observer> observers;

	public Portfolio() {
		this.stocks = new ArrayList<>();
		this.accounts = new ArrayList<>();
		this.observers = new ArrayList<>();
	}

	public ArrayList<Stock> getStocks() {
		return stocks;
	}

	public Stock getStock(String stockName) {
		for (Stock stock : stocks)
			if (stock.getStockName()
				.equals(stockName))
				return stock;

		return null;
	}

	public ArrayList<Account> getAccounts() {
		return accounts;
	}

	public Account getAccount(CurrencyName currencyName) {
		for (Account account : accounts)
			if (account.getCurrencyName()
				.equals(currencyName))
				return account;

		return null;
	}

	@Override
	public void registerObserver(Observer observer) {
		observers.add(observer);
	}

	@Override
	public void notifyObservers() {
		observers.forEach(Observer::update);
	}
}
