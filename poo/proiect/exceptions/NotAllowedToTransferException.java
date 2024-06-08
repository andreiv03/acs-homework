package org.poo.cb.exceptions;

public class NotAllowedToTransferException extends Exception {
	public NotAllowedToTransferException(String email) {
		super("You are not allowed to transfer money to " + email);
	}
}
