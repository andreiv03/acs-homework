package org.example.actions;

import org.example.IMDB;
import org.example.exceptions.InvalidOptionException;
import org.example.interfaces.Observer;
import org.example.production.Production;
import org.example.production.Rating;
import org.example.strategies.ReviewAdditionStrategy;
import org.example.user.User;

import java.util.ArrayList;
import java.util.stream.Collectors;

public class AddDeleteReviewsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();
			ArrayList<Rating> ratings = new ArrayList<>();

			for (Production production : imdb.getProductions())
				ratings.addAll(production.getRatings().stream().filter(
						rating -> rating.getUsername().equals(currentUser.getUsername())).collect(
						Collectors.toCollection(ArrayList::new)));

			System.out.println("Your ratings: " + ratings);
			System.out.println();

			System.out.println("What do you want to do?");
			System.out.println("\t1) Add");
			System.out.println("\t2) Delete");
			System.out.println("\t3) Exit");

			int action = getIntegerUserInput(3);

			if (action == 3)
				return;

			String name = getStringUserInput("Enter the name: ");

			switch (action) {
				case 1:
					if (imdb.getProductions().stream().noneMatch(production -> production.getTitle().equals(name))) {
						System.out.println("Production not found!");
						System.out.println();
						return;
					}

					if (ratings.stream().anyMatch(rating -> rating.getProductionTitle().equals(name))) {
						System.out.println("You have already reviewed this production!");
						System.out.println();
						return;
					}

					String rating = getStringUserInput("Enter the rating (1-10): ");

					if (Integer.parseInt(rating) < 1 || Integer.parseInt(rating) > 10) {
						System.out.println("Invalid rating!");
						System.out.println();
						return;
					}

					String review = getStringUserInput("Enter the review: ");

					for (Production production : imdb.getProductions()) {
						if (production.getTitle().equals(name)) {
							Rating newRating = new Rating();
							newRating.setUsername(currentUser.getUsername());
							newRating.setRating(Integer.parseInt(rating));
							newRating.setComment(review);
							newRating.setProductionTitle(name);
							production.getRatings().add(newRating);
							production.notifyObservers();
							production.registerObserver((Observer) currentUser);

							if (!currentUser.getReviewedProductions().contains(production.getTitle()))
								currentUser.updateExperience(new ReviewAdditionStrategy());

							currentUser.getReviewedProductions().add(production.getTitle());
						}
					}

					break;

				case 2:
					if (ratings.stream().noneMatch(r -> r.getProductionTitle().equals(name))) {
						System.out.println("You have not reviewed this production!");
						System.out.println();
						return;
					}

					for (Production production : imdb.getProductions())
						if (production.getTitle().equals(name))
							production.getRatings().removeIf(r -> r.getUsername().equals(currentUser.getUsername()));
			}
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
