package org.poo.cb.exceptions;

public class UserDoesNotExistException extends Exception {
	public UserDoesNotExistException(String email) {
		super("User with " + email + " doesn't exist");
	}
}
