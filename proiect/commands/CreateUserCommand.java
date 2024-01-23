package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.exceptions.UserAlreadyExistsException;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class CreateUserCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserAlreadyExistsException {
		String email = arguments.get(0);
		String firstName = arguments.get(1);
		String lastName = arguments.get(2);
		String address = String.join(" ", arguments.subList(3, arguments.size()));

		if (Main.users.containsKey(email))
			throw new UserAlreadyExistsException(email);

		User user = new User.UserBuilder().setEmail(email)
			.setFirstName(firstName)
			.setLastName(lastName)
			.setAddress(address)
			.build();

		Main.users.put(email, user);
	}
}
