package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountDoesNotExistException;
import org.poo.cb.exceptions.InsufficientAmountForStocksException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.*;

import java.util.ArrayList;

public class BuyStocksCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, AccountDoesNotExistException,
		InsufficientAmountForStocksException {
		String email = arguments.get(0);
		String stockName = arguments.get(1);
		int numberOfStocks = Integer.parseInt(arguments.get(2));

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		Portfolio portfolio = user.getPortfolio();
		Account account = portfolio.getAccount(CurrencyName.USD);

		if (account == null)
			throw new AccountDoesNotExistException(CurrencyName.USD);

		if (portfolio.getStock(stockName) == null)
			portfolio.getStocks()
				.add(new Stock(stockName, 0));

		Stock stock = portfolio.getStock(stockName);
		double stockLatestValue = stock.findStockLatestValue();
		double amount = numberOfStocks * stockLatestValue;

		if (user.hasPremium()) {
			RecommendedStocks recommendedStocks = RecommendedStocks.getInstance();

			if (recommendedStocks.getStocksToBuy()
				.contains(stockName))
				amount = amount - (amount * 0.05);
		}

		if (account.getAmount() < amount)
			throw new InsufficientAmountForStocksException();

		account.withdraw(amount);
		stock.addStocks(numberOfStocks);
	}
}
