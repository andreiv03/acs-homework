package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.enumerations.CurrencyName;
import org.poo.cb.exceptions.AccountDoesNotExistException;
import org.poo.cb.exceptions.InsufficientAmountForTransferException;
import org.poo.cb.exceptions.NotAllowedToTransferException;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.Account;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class TransferMoneyCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, NotAllowedToTransferException,
		AccountDoesNotExistException, InsufficientAmountForTransferException {
		String userEmail = arguments.get(0);
		String friendEmail = arguments.get(1);
		CurrencyName currencyName = CurrencyName.valueOf(arguments.get(2));
		double amount = Double.parseDouble(arguments.get(3));

		User user = Main.users.get(userEmail);

		if (user == null)
			throw new UserDoesNotExistException(userEmail);

		User friend = Main.users.get(friendEmail);

		if (friend == null)
			throw new UserDoesNotExistException(friendEmail);

		if (!user.getFriends()
			.contains(friendEmail))
			throw new NotAllowedToTransferException(friendEmail);

		Account userAccount = user.getPortfolio()
			.getAccount(currencyName);

		if (userAccount == null)
			throw new AccountDoesNotExistException(currencyName);

		Account friendAccount = friend.getPortfolio()
			.getAccount(currencyName);

		if (friendAccount == null)
			throw new AccountDoesNotExistException(currencyName);

		if (userAccount.getAmount() < amount)
			throw new InsufficientAmountForTransferException(currencyName);

		userAccount.withdraw(amount);
		friendAccount.deposit(amount);
	}
}
