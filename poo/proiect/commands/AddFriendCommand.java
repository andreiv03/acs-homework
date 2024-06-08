package org.poo.cb.commands;

import org.poo.cb.Main;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.exceptions.UserIsAlreadyFriendException;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class AddFriendCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, UserIsAlreadyFriendException {
		String userEmail = arguments.get(0);
		String friendEmail = arguments.get(1);

		User user = Main.users.get(userEmail);

		if (user == null)
			throw new UserDoesNotExistException(userEmail);

		User friend = Main.users.get(friendEmail);

		if (friend == null)
			throw new UserDoesNotExistException(friendEmail);

		if (user.getFriends()
			.contains(friendEmail))
			throw new UserIsAlreadyFriendException(friendEmail);

		user.getFriends()
			.add(friendEmail);
		friend.getFriends()
			.add(userEmail);
	}
}
