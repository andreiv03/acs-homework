package org.example.actions;

import org.example.exceptions.InvalidOptionException;
import org.example.user.User;

import java.util.Scanner;

public abstract class Action {
	public int getIntegerUserInput(int maximumNumberOfChoices) throws InvalidOptionException {
		Scanner scanner = new Scanner(System.in);

		System.out.print("Your option: ");
		int option = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (option < 1 || option > maximumNumberOfChoices)
			throw new InvalidOptionException("Invalid option!");

		return option;
	}

	public String getStringUserInput(String message) {
		Scanner scanner = new Scanner(System.in);

		System.out.print(message);
		String option = scanner.nextLine();
		System.out.println();

		return option;
	}

	abstract public void execute(User currentUser);
}
