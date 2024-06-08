package org.example.actions;

import org.example.IMDB;
import org.example.enumerations.Genre;
import org.example.exceptions.InvalidOptionException;
import org.example.production.Production;
import org.example.user.User;

import java.util.ArrayList;

public class ViewProductionsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();
			ArrayList<Production> productions = new ArrayList<>(imdb.getProductions());

			System.out.println("Do you want to sort the productions?");
			System.out.println("\t1) Yes");
			System.out.println("\t2) No");
			System.out.println("\t3) Exit");

			int canSortProductions = getIntegerUserInput(3);

			if (canSortProductions == 3)
				return;

			System.out.println("Do you want to filter the productions?");
			System.out.println("\t1) Yes");
			System.out.println("\t2) No");
			System.out.println("\t3) Exit");

			int canFilterProductions = getIntegerUserInput(3);

			if (canFilterProductions == 3)
				return;

			if (canFilterProductions == 1) {
				System.out.println("What do you want to filter by?");
				System.out.println("\t1) Genre");
				System.out.println("\t2) Type");
				System.out.println("\t3) Rating");
				System.out.println("\t4) Number of ratings");
				System.out.println("\t5) Number of directors");
				System.out.println("\t6) Number of actors");
				System.out.println("\t7) Number of genres");
				System.out.println("\t8) Exit");

				int filterBy = getIntegerUserInput(8);

				if (filterBy == 8)
					return;

				switch (filterBy) {
					case 1:
						String genre = getStringUserInput("Enter the genre: ");

						if (!Genre.contains(genre)) {
							System.out.println("Invalid genre!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> production.getGenres().contains(Genre.valueOf(genre)));
						break;

					case 2:
						System.out.println("Choose the type: ");
						System.out.println("\t1) Movie");
						System.out.println("\t2) Series");
						System.out.println("\t3) Exit");

						int type = getIntegerUserInput(3);

						if (type == 3)
							return;

						productions.removeIf(production -> production.getType().equals(type == 1 ? "Series" : "Movie"));
						break;

					case 3:
						String rating = getStringUserInput("Enter the rating [1-10]: ");

						if (Double.parseDouble(rating) < 1 || Double.parseDouble(rating) > 10) {
							System.out.println("Invalid rating!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> (int) production.getAverageRating() != (int) Double.parseDouble(rating));
						break;

					case 4:
						String numberOfRatings = getStringUserInput("Enter the number of ratings: ");

						if (Integer.parseInt(numberOfRatings) < 1) {
							System.out.println("Invalid number of ratings!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> production.getRatings().size() < Integer.parseInt(numberOfRatings));
						break;

					case 5:
						String numberOfDirectors = getStringUserInput("Enter the number of directors: ");

						if (Integer.parseInt(numberOfDirectors) < 1) {
							System.out.println("Invalid number of directors!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> production.getDirectors().size() < Integer.parseInt(numberOfDirectors));
						break;

					case 6:
						String numberOfActors = getStringUserInput("Enter the number of actors: ");

						if (Integer.parseInt(numberOfActors) < 1) {
							System.out.println("Invalid number of actors!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> production.getActors().size() < Integer.parseInt(numberOfActors));
						break;

					case 7:
						String numberOfGenres = getStringUserInput("Enter the number of genres: ");

						if (Integer.parseInt(numberOfGenres) < 1) {
							System.out.println("Invalid number of genres!");
							System.out.println();
							return;
						}

						productions.removeIf(production -> production.getGenres().size() < Integer.parseInt(numberOfGenres));
						break;
				}
			}

			if (canSortProductions == 1)
				productions.sort(null);

			System.out.println("Productions:");

			for (Production production : productions)
				production.displayInfo();
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}