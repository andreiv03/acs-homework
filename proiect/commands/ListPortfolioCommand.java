package org.poo.cb.commands;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;
import org.poo.cb.Main;
import org.poo.cb.exceptions.UserDoesNotExistException;
import org.poo.cb.model.User;
import org.poo.cb.serializers.CustomDoubleSerializer;

import java.util.ArrayList;

public class ListPortfolioCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws UserDoesNotExistException, JsonProcessingException {
		String email = arguments.get(0);

		User user = Main.users.get(email);

		if (user == null)
			throw new UserDoesNotExistException(email);

		ObjectMapper objectMapper = new ObjectMapper();
		SimpleModule simpleModule = new SimpleModule();
		simpleModule.addSerializer(Double.class, new CustomDoubleSerializer());
		objectMapper.registerModule(simpleModule);

		String portfolioJson = objectMapper.writeValueAsString(user.getPortfolio());
		System.out.println(portfolioJson);
	}
}
