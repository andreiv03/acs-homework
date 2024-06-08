package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountDoesNotExistException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.Account;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class AddMoneyCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, AccountDoesNotExistException {
		String email = arguments.get(0);
		CurrencyName currencyName = CurrencyName.valueOf(arguments.get(1));
		double amount = Double.parseDouble(arguments.get(2));

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		Account account = user.getPortfolio()
			.getAccount(currencyName);

		if (account == null)
			throw new AccountDoesNotExistException(currencyName);

		account.deposit(amount);
	}
}
