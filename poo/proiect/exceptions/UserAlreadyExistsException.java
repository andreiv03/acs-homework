package org.poo.cb.exceptions;

public class UserAlreadyExistsException extends Exception {
	public UserAlreadyExistsException(String email) {
		super("User with " + email + " already exists");
	}
}
