package org.poo.cb.commands;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.poo.cb.model.RecommendedStocks;

import java.util.ArrayList;

public class RecommendStocksCommand extends Command {
	@Override
	public void execute(ArrayList<String> arguments) throws JsonProcessingException {
		RecommendedStocks recommendedStocks = RecommendedStocks.getInstance();
		recommendedStocks.determineStocksToBuy();

		ObjectMapper objectMapper = new ObjectMapper();

		String recommendedStocksJson = objectMapper.writeValueAsString(recommendedStocks);
		System.out.println(recommendedStocksJson);
	}
}
