package org.poo.cb.exceptions;

import org.poo.cb.enumerations.CurrencyName;

public class AccountAlreadyExistsException extends Exception {
	public AccountAlreadyExistsException(CurrencyName currencyName) {
		super("Account in currency " + currencyName.name() + " already exists for user");
	}
}
