package org.example.actions;

import org.example.IMDB;
import org.example.actor.Actor;
import org.example.enumerations.AccountType;
import org.example.exceptions.InvalidOptionException;
import org.example.interfaces.Observer;
import org.example.production.Movie;
import org.example.production.Production;
import org.example.production.Series;
import org.example.strategies.ActorAdditionStrategy;
import org.example.strategies.ProductionAdditionStrategy;
import org.example.user.Staff;
import org.example.user.User;

public class AddDeleteActorsProductionsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();

			System.out.println("What do you want to do?");
			System.out.println("\t1) Add");
			System.out.println("\t2) Delete");
			System.out.println("\t3) Exit");

			int action = getIntegerUserInput(3);

			if (action == 3)
				return;

			System.out.println("What do you want to " + (action == 1 ? "add" : "delete") + "?");
			System.out.println("\t1) Actor");
			System.out.println("\t2) Movie");
			System.out.println("\t3) Series");
			System.out.println("\t4) Exit");

			int choice = getIntegerUserInput(4);

			if (choice == 4)
				return;

			if (action == 2) {
				if (choice == 1) {
					System.out.print("Actors: [");

					for (Actor actor : imdb.getActors()) {
						System.out.print(actor.getName());

						if (actor != imdb.getActors().get(imdb.getActors().size() - 1))
							System.out.print(", ");
					}

					System.out.println("]");
					System.out.println();
				}

				if (choice == 2 || choice == 3) {
					System.out.print("Productions: [");

					for (Production production : imdb.getProductions()) {
						System.out.print(production.getTitle());

						if (production != imdb.getProductions().get(imdb.getProductions().size() - 1))
							System.out.print(", ");
					}

					System.out.println("]");
					System.out.println();
				}
			}

			String name = getStringUserInput("Enter the name: ");

			switch (choice) {
				case 1:
					if (action == 1) {
						Actor newActor = new Actor();
						newActor.setName(name);
						newActor.setResponsible(currentUser.getUsername());
						((Staff) currentUser).addActorSystem(newActor);
						currentUser.updateExperience(new ActorAdditionStrategy());
						break;
					}

					Actor actor =
							imdb.getActors().stream().filter(a -> a.getName().equals(name)).findFirst().orElse(null);

					if (actor == null) {
						System.out.println("Actor not found!");
						System.out.println();
						return;
					}

					if (!actor.getResponsible().equals("ADMIN") && !actor.getResponsible().equals(currentUser.getUsername())) {
						System.out.println("You are not responsible for this actor!");
						System.out.println();
						return;
					}

					if (actor.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
						System.out.println("You are not an admin!");
						System.out.println();
						return;
					}

					((Staff) currentUser).removeActorSystem(name);
					break;

				case 2:
					if (action == 1) {
						Movie newMovie = new Movie();
						newMovie.setTitle(name);
						newMovie.setResponsible(currentUser.getUsername());
						newMovie.registerObserver((Observer) currentUser);
						((Staff) currentUser).addProductionSystem(newMovie);
						currentUser.updateExperience(new ProductionAdditionStrategy());
						break;
					}

					Production movie = imdb.getProductions().stream().filter(
							p -> p.getType().equals("Movie") && p.getTitle().equals(name)).findFirst().orElse(null);

					if (movie == null) {
						System.out.println("Movie not found!");
						System.out.println();
						return;
					}

					if (!movie.getResponsible().equals(currentUser.getUsername())) {
						System.out.println("You are not responsible for this movie!");
						System.out.println();
						return;
					}

					if (movie.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
						System.out.println("You are not an admin!");
						System.out.println();
						return;
					}

					((Staff) currentUser).removeProductionSystem(name);
					break;

				case 3:
					if (action == 1) {
						Series newSeries = new Series();
						newSeries.setTitle(name);
						newSeries.setResponsible(currentUser.getUsername());
						newSeries.registerObserver((Observer) currentUser);
						((Staff) currentUser).addProductionSystem(newSeries);
						currentUser.updateExperience(new ProductionAdditionStrategy());
						break;
					}

					Production series = imdb.getProductions().stream().filter(
							p -> p.getType().equals("Series") && p.getTitle().equals(name)).findFirst().orElse(null);

					if (series == null) {
						System.out.println("Series not found!");
						System.out.println();
						return;
					}

					if (!series.getResponsible().equals(currentUser.getUsername())) {
						System.out.println("You are not responsible for this series!");
						System.out.println();
						return;
					}

					if (series.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
						System.out.println("You are not an admin!");
						System.out.println();
						return;
					}

					((Staff) currentUser).removeProductionSystem(name);
					break;
			}
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
