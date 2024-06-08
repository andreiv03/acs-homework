package org.poo.cb.commands;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class CommandInvoker {
	public static volatile CommandInvoker commandInvoker = null;

	private CommandInvoker() {
		if (commandInvoker != null)
			throw new RuntimeException("Use getInstance() method to get the single instance of this class!");
	}

	public static CommandInvoker getInstance() {
		if (commandInvoker == null) {
			synchronized (CommandInvoker.class) {
				if (commandInvoker == null)
					commandInvoker = new CommandInvoker();
			}
		}

		return commandInvoker;
	}

	public void executeCommand(String command) {
		String[] commandParts = command.split(" ");

		String commandName = Arrays.stream(commandParts)
			.takeWhile(part -> part.equals(part.toUpperCase()))
			.collect(Collectors.joining(" "));

		ArrayList<String> arguments = new ArrayList<>();

		if (commandParts.length > commandName.split(" ").length)
			arguments = new ArrayList<>(List.of(commandParts)
				.subList(commandName.split(" ").length, commandParts.length));

		try {
			switch (commandName) {
				case "CREATE USER" -> new CreateUserCommand().execute(arguments);
				case "ADD FRIEND" -> new AddFriendCommand().execute(arguments);
				case "ADD ACCOUNT" -> new AddAccountCommand().execute(arguments);
				case "ADD MONEY" -> new AddMoneyCommand().execute(arguments);
				case "EXCHANGE MONEY" -> new ExchangeMoneyCommand().execute(arguments);
				case "TRANSFER MONEY" -> new TransferMoneyCommand().execute(arguments);
				case "BUY STOCKS" -> new BuyStocksCommand().execute(arguments);
				case "RECOMMEND STOCKS" -> new RecommendStocksCommand().execute(arguments);
				case "LIST USER" -> new ListUserCommand().execute(arguments);
				case "LIST PORTFOLIO" -> new ListPortfolioCommand().execute(arguments);
				case "BUY PREMIUM" -> new BuyPremiumCommand().execute(arguments);
				default -> throw new IllegalArgumentException("Invalid command");
			}
		} catch (Exception exception) {
			System.out.println(exception.getMessage());
		}
	}
}
