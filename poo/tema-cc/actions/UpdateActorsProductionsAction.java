package org.example.actions;

import org.example.IMDB;
import org.example.actor.Actor;
import org.example.exceptions.InvalidOptionException;
import org.example.production.Movie;
import org.example.production.Production;
import org.example.production.Series;
import org.example.user.Staff;
import org.example.user.User;

public class UpdateActorsProductionsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();

			System.out.println("What do you want to update?");
			System.out.println("\t1) Actor");
			System.out.println("\t2) Movie");
			System.out.println("\t3) Series");
			System.out.println("\t4) Exit");

			int choice = getIntegerUserInput(4);

			if (choice == 4)
				return;

			String name = getStringUserInput("Enter the name: ");

			switch (choice) {
				case 1: {
					Actor updatedActor = null;

					for (Actor actor : imdb.getActors()) {
						if (actor.getName().equals(name)) {
							updatedActor = actor;
							break;
						}
					}

					if (updatedActor == null) {
						System.out.println("Actor not found!");
						System.out.println();
						return;
					}

					System.out.println("What do you want to update?");
					System.out.println("\t1) Name");
					System.out.println("\t2) Biography");
					System.out.println("\t3) Exit");

					int choiceTwo = getIntegerUserInput(3);

					if (choiceTwo == 3)
						return;

					switch (choiceTwo) {
						case 1:
							String newName = getStringUserInput("Enter the new name: ");
							updatedActor.setName(newName);
							break;

						case 2:
							String newBiography = getStringUserInput("Enter the new biography: ");
							updatedActor.setBiography(newBiography);
							break;
					}

					((Staff) currentUser).updateActorSystem(updatedActor);
					break;
				}

				case 2: {
					Movie updatedMovie = null;

					for (Production production : imdb.getProductions()) {
						if (production.getType().equals("Movie") && production.getTitle().equals(name)) {
							updatedMovie = (Movie) production;
							break;
						}
					}

					if (updatedMovie == null) {
						System.out.println("Movie not found!");
						System.out.println();
						return;
					}

					System.out.println("What do you want to update?");
					System.out.println("\t1) Title");
					System.out.println("\t2) Plot");
					System.out.println("\t3) Exit");

					int choiceTwo = getIntegerUserInput(3);

					if (choiceTwo == 3)
						return;

					switch (choiceTwo) {
						case 1:
							String newTitle = getStringUserInput("Enter the new title: ");
							updatedMovie.setTitle(newTitle);
							break;

						case 2:
							String newPlot = getStringUserInput("Enter the new plot: ");
							updatedMovie.setPlot(newPlot);
							break;
					}

					((Staff) currentUser).updateProductionSystem(updatedMovie);
					break;
				}

				case 3: {
					Series updatedSeries = null;

					for (Production production : imdb.getProductions()) {
						if (production.getType().equals("Series") && production.getTitle().equals(name)) {
							updatedSeries = (Series) production;
							break;
						}
					}

					if (updatedSeries == null) {
						System.out.println("Series not found!");
						System.out.println();
						return;
					}

					System.out.println("What do you want to update?");
					System.out.println("\t1) Title");
					System.out.println("\t2) Plot");
					System.out.println("\t3) Exit");

					int choiceTwo = getIntegerUserInput(3);

					if (choiceTwo == 3)
						return;

					switch (choiceTwo) {
						case 1:
							String newTitle = getStringUserInput("Enter the new title: ");
							updatedSeries.setTitle(newTitle);
							break;

						case 2:
							String newPlot = getStringUserInput("Enter the new plot: ");
							updatedSeries.setPlot(newPlot);
							break;
					}

					((Staff) currentUser).updateProductionSystem(updatedSeries);
					break;
				}
			}
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
