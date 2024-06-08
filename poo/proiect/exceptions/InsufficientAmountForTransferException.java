package org.poo.cb.exceptions;

import org.poo.cb.enumerations.CurrencyName;

public class InsufficientAmountForTransferException extends Exception {
	public InsufficientAmountForTransferException(CurrencyName currencyName) {
		super("Insufficient amount in account " + currencyName.name() + " for transfer");
	}
}
