package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountDoesNotExistException;
import org.poo.cb.exceptions.InsufficientAmountForPremiumException;
import org.poo.cb.exceptions.UserAlreadyHasPremiumException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.Account;
import org.poo.cb.model.Portfolio;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class BuyPremiumCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, UserAlreadyHasPremiumException,
		AccountDoesNotExistException, InsufficientAmountForPremiumException {
		String email = arguments.get(0);

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		if (user.hasPremium())
			throw new UserAlreadyHasPremiumException(email);

		Portfolio portfolio = user.getPortfolio();
		Account account = portfolio.getAccount(CurrencyName.USD);

		if (account == null)
			throw new AccountDoesNotExistException(CurrencyName.USD);

		if (account.getAmount() < 100)
			throw new InsufficientAmountForPremiumException();

		user.upgradeToPremium();
		portfolio.notifyObservers();
		account.withdraw(100);
	}
}
