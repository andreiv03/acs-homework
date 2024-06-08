package org.example.exceptions;

public class StudentAlreadyExists extends Exception {
	public StudentAlreadyExists(String message) {
		super(message);
	}
}
