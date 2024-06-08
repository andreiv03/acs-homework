package org.poo.cb.exceptions;

public class InsufficientAmountForStocksException extends Exception {
	public InsufficientAmountForStocksException() {
		super("Insufficient amount in account for buying stock");
	}
}
