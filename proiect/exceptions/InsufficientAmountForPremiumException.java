package org.poo.cb.exceptions;

public class InsufficientAmountForPremiumException extends Exception {
	public InsufficientAmountForPremiumException() {
		super("Insufficient amount in account for buying premium option");
	}
}
