package org.poo.cb.exceptions;

import org.poo.cb.enumerations.CurrencyName;

public class InsufficientAmountForExchangeException extends Exception {
	public InsufficientAmountForExchangeException(CurrencyName currencyName) {
		super("Insufficient amount in account " + currencyName.name() + " for exchange");
	}
}
