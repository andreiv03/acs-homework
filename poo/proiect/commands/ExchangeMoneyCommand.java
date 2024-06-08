package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountDoesNotExistException;
import org.poo.cb.exceptions.InsufficientAmountForExchangeException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.Account;
import org.poo.cb.model.Portfolio;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class ExchangeMoneyCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, AccountDoesNotExistException,
		InsufficientAmountForExchangeException {
		String email = arguments.get(0);
		CurrencyName sourceCurrencyName = CurrencyName.valueOf(arguments.get(1));
		CurrencyName destinationCurrencyName = CurrencyName.valueOf(arguments.get(2));
		double amount = Double.parseDouble(arguments.get(3));

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		Portfolio portfolio = user.getPortfolio();
		Account sourceAccount = portfolio.getAccount(sourceCurrencyName);

		if (sourceAccount == null)
			throw new AccountDoesNotExistException(sourceCurrencyName);

		Account destinationAccount = portfolio.getAccount(destinationCurrencyName);

		if (destinationAccount == null)
			throw new AccountDoesNotExistException(destinationCurrencyName);

		if (sourceAccount.getAmount() < amount)
			throw new InsufficientAmountForExchangeException(sourceCurrencyName);

		double exchangeRate = destinationAccount.getExchangeRate(sourceCurrencyName);
		double commissionFee = sourceAccount.getCommissionFee(amount * exchangeRate);

		sourceAccount.withdraw(amount * exchangeRate + commissionFee);
		destinationAccount.deposit(amount);
	}
}
