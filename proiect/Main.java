package org.poo.cb;

import org.poo.cb.commands.CommandInvoker;
import org.poo.cb.model.RecommendedStocks;
import org.poo.cb.model.User;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;

public class Main {
	public static ArrayList<String> exchangeRates;
	public static ArrayList<String> stockValues;
	public static ArrayList<String> commands;

	public static HashMap<String, User> users;

	public static void main(String[] args) {
		if (args == null) {
			System.out.println("Running Main");
			return;
		}

		users = new HashMap<>();

		try {
			Path resourcesFolderPath = Paths.get("src/main/resources/");

			Path exchangeRatesFilePath = resourcesFolderPath.resolve(args[0]);
			exchangeRates = new ArrayList<>(Files.readAllLines(exchangeRatesFilePath));

			Path stockValuesFilePath = resourcesFolderPath.resolve(args[1]);
			stockValues = new ArrayList<>(Files.readAllLines(stockValuesFilePath));

			Path commandsFilePath = resourcesFolderPath.resolve(args[2]);
			commands = new ArrayList<>(Files.readAllLines(commandsFilePath));
		} catch (Exception exception) {
			System.out.println(exception.getMessage());
		}

		CommandInvoker commandInvoker = CommandInvoker.getInstance();
		commands.forEach(commandInvoker::executeCommand);

		RecommendedStocks.getInstance()
			.resetStocksToBuy();
	}
}
