package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountAlreadyExistsException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.Account;
import org.poo.cb.model.Portfolio;
import org.poo.cb.model.User;
import org.poo.cb.strategies.PremiumCommissionFeeStrategy;

import java.util.ArrayList;

public class AddAccountCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, AccountAlreadyExistsException {
		String email = arguments.get(0);
		CurrencyName currencyName = CurrencyName.valueOf(arguments.get(1));

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		Portfolio portfolio = user.getPortfolio();

		if (portfolio.getAccount(currencyName) != null)
			throw new AccountAlreadyExistsException(currencyName);

		Account account = new Account(currencyName, 0);

		if (user.hasPremium())
			account.setCommissionFeeStrategy(new PremiumCommissionFeeStrategy());

		portfolio.registerObserver(account);
		portfolio.getAccounts()
			.add(account);
	}
}
