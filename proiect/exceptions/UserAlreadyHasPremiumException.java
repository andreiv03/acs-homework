package org.poo.cb.exceptions;

public class UserAlreadyHasPremiumException extends Exception {
	public UserAlreadyHasPremiumException(String email) {
		super("User with " + email + " already has premium");
	}
}
