package org.poo.cb.exceptions;

import org.poo.cb.enumerations.CurrencyName;

public class AccountDoesNotExistException extends Exception {
	public AccountDoesNotExistException(CurrencyName currencyName) {
		super("Account in currency " + currencyName.name() + " doesn't exist for user");
	}
}
