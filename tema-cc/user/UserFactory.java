package org.example.user;

import org.example.enumerations.AccountType;

public class UserFactory {
	public static User createUser(AccountType accountType) {
		return switch (accountType) {
			case Regular -> new Regular();
			case Contributor -> new Contributor();
			case Admin -> new Admin();
		};
	}

	public static User createUser(User user) {
		return switch (user.getUserType()) {
			case Regular -> new Regular(user);
			case Contributor -> new Contributor(user);
			case Admin -> new Admin(user);
		};
	}
}
