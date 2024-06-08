package org.example.actions;

import org.example.IMDB;
import org.example.actor.Actor;
import org.example.enumerations.AccountType;
import org.example.exceptions.InformationIncompleteException;
import org.example.exceptions.InvalidOptionException;
import org.example.interfaces.Observer;
import org.example.production.Production;
import org.example.request.Request;
import org.example.user.Credentials;
import org.example.user.User;
import org.example.user.UserFactory;
import org.example.utils.Utils;

public class AddDeleteUsersAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();

			System.out.print("Users: [");

			for (User user : imdb.getUsers()) {
				System.out.print(user.getUsername());

				if (user != imdb.getUsers().get(imdb.getUsers().size() - 1))
					System.out.print(", ");
			}

			System.out.println("]");
			System.out.println();

			System.out.println("What do you want to do?");
			System.out.println("\t1) Add");
			System.out.println("\t2) Delete");
			System.out.println("\t3) Exit");

			int action = getIntegerUserInput(3);

			if (action == 3)
				return;

			String name = getStringUserInput("Enter the " + (action == 1 ? "name" : "username") + ": ");

			switch (action) {
				case 1:
					String firstName = name.split(" ")[0];
					String lastName = name.split(" ")[1];
					String username = Utils.generateUsername(firstName, lastName);
					String email = Utils.generateEmail(firstName, lastName);
					String password = Utils.generateStrongPassword(12);

					User newUser = UserFactory.createUser(AccountType.Regular);

					if (email.isEmpty() || password.isEmpty() || name.isEmpty())
						throw new InformationIncompleteException("Information is incomplete!");

					User.Information.InformationBuilder informationBuilder =
							new User.Information.InformationBuilder().setCredentials(
							new Credentials(email, password)).setName(name);

					newUser.setUsername(username);
					newUser.setInformation(informationBuilder.build());
					imdb.getUsers().add(newUser);

					System.out.println("The credentials are:");
					System.out.println("\temail: " + email);
					System.out.println("\tpassword: " + password);
					System.out.println();
					break;

				case 2:
					if (name.equals(currentUser.getUsername())) {
						System.out.println("You cannot delete yourself!");
						System.out.println();
						return;
					}

					User user = imdb.getUsers().stream().filter(u -> u.getUsername().equals(name)).findFirst().orElse(null);

					if (user == null) {
						System.out.println("User not found!");
						System.out.println();
						return;
					}

					if (user.getUserType() == AccountType.Regular)
						for (Production production : imdb.getProductions())
							production.getRatings().removeIf(rating -> rating.getUsername().equals(user.getUsername()));

					if (user.getUserType() == AccountType.Contributor) {
						for (Actor actor : imdb.getActors())
							if (actor.getResponsible().equals(user.getUsername()))
								actor.setResponsible("ADMIN");

						for (Production production : imdb.getProductions())
							if (production.getResponsible().equals(user.getUsername()))
								production.setResponsible("ADMIN");
					}

					if (user.getUserType() == AccountType.Admin) {
						System.out.println("You cannot delete an admin!");
						System.out.println();
						return;
					}

					imdb.getRequests().removeIf(request -> request.getUsername().equals(user.getUsername()));

					for (Production production : imdb.getProductions())
						production.removeObserver((Observer) user);

					for (Request request : imdb.getRequests())
						request.removeObserver((Observer) user);

					imdb.getUsers().remove(user);
					break;
			}
		} catch (InvalidOptionException | InformationIncompleteException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
