package org.poo.cb.exceptions;

public class UserIsAlreadyFriendException extends Exception {
	public UserIsAlreadyFriendException(String email) {
		super("User with " + email + " is already a friend");
	}
}
