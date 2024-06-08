package org.example.utils;

import java.security.SecureRandom;

public class Utils {
	public static String generateUsername(String firstName, String lastName) {
		int randomNumber = new SecureRandom().nextInt(1000);
		return (firstName + "_" + lastName + "_" + randomNumber).toLowerCase();
	}

	public static String generateEmail(String firstName, String lastName) {
		return (firstName + "_" + lastName + "@example.com").toLowerCase();
	}

	public static String generateStrongPassword(int length) {
		String upperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		String lowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
		String numbers = "0123456789";
		String specialCharacters = "!@#$%^&*";
		String combinedChars = upperCaseLetters + lowerCaseLetters + numbers + specialCharacters;
		SecureRandom random = new SecureRandom();
		StringBuilder password = new StringBuilder(length);

		password.append(upperCaseLetters.charAt(random.nextInt(upperCaseLetters.length())));
		password.append(lowerCaseLetters.charAt(random.nextInt(lowerCaseLetters.length())));
		password.append(numbers.charAt(random.nextInt(numbers.length())));
		password.append(specialCharacters.charAt(random.nextInt(specialCharacters.length())));

		for (int index = 4; index < length; ++index)
			password.append(combinedChars.charAt(random.nextInt(combinedChars.length())));

		char[] passwordArray = password.toString().toCharArray();

		for (int index = passwordArray.length - 1; index > 0; --index) {
			int randomIndex = random.nextInt(index + 1);
			char aux = passwordArray[randomIndex];
			passwordArray[randomIndex] = passwordArray[index];
			passwordArray[index] = aux;
		}

		return new String(passwordArray);
	}

	public static String generateId() {
		return String.valueOf(new SecureRandom().nextInt(1000000));
	}
}
