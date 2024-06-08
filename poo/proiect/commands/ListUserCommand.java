package org.poo.cb.commands;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.poo.cb.Main;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.User;

import java.util.ArrayList;

public class ListUserCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, JsonProcessingException {
		String email = arguments.get(0);

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		ObjectMapper objectMapper = new ObjectMapper();

		String userJson = objectMapper.writeValueAsString(user);
		System.out.println(userJson);
	}
}
