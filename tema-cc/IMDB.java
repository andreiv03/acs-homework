package org.example;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.example.actions.*;
import org.example.actor.Actor;
import org.example.enumerations.AccountType;
import org.example.exceptions.InvalidCommandException;
import org.example.interfaces.Observer;
import org.example.production.Production;
import org.example.production.Rating;
import org.example.request.Request;
import org.example.request.RequestsHolder;
import org.example.user.Credentials;
import org.example.user.User;
import org.example.user.UserFactory;

import java.io.File;
import java.io.IOException;
import java.util.*;

public class IMDB {
	private static volatile IMDB imdb = null;

	private ArrayList<User> users;
	private ArrayList<Actor> actors;
	private ArrayList<Request> requests;
	private ArrayList<Production> productions;

	private IMDB() {
		if (imdb != null)
			throw new RuntimeException("There is already an instance of IMDB!");
	}

	public static IMDB getInstance() {
		if (imdb == null) {
			synchronized (IMDB.class) {
				if (imdb == null)
					imdb = new IMDB();
			}
		}

		return imdb;
	}

	public static void main(String[] args) {
		IMDB imdb = IMDB.getInstance();
		imdb.run();
	}

	public static <T> ArrayList<T> readArrayOfObjectsFromJSON(String path, TypeReference<ArrayList<T>> typeReference) {
		ObjectMapper objectMapper = new ObjectMapper();
		objectMapper.findAndRegisterModules();
		objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
		objectMapper.configure(DeserializationFeature.ACCEPT_SINGLE_VALUE_AS_ARRAY, true);

		try {
			return objectMapper.readValue(new File(path), typeReference);
		} catch (IOException exception) {
			System.out.println(exception.getMessage());
			return null;
		}
	}

	public ArrayList<User> getUsers() {
		return users;
	}

	public ArrayList<Actor> getActors() {
		return actors;
	}

	public ArrayList<Request> getRequests() {
		return requests;
	}

	public ArrayList<Production> getProductions() {
		return productions;
	}

	public void run() {
		try {
			readFromDatabase();
			boolean isRunning = true;

			while (isRunning) {
				User currentUser = login();

				if (currentUser == null)
					continue;

				System.out.println();
				System.out.println("Welcome back, " + currentUser.getUsername() + "!");
				System.out.println("You are logged in as " + currentUser.getUserType() + ".");
				System.out.println();

				isRunning = startApplicationFlow(currentUser);
			}
		} catch (InvalidCommandException exception) {
			System.out.println(exception.getMessage());
		}
	}

	public void readFromDatabase() {
		String inputPath = "src/main/resources/input/";

		TypeReference<ArrayList<User>> usersTypeReference = new TypeReference<>() {
		};
		this.users = readArrayOfObjectsFromJSON(inputPath + "accounts.json", usersTypeReference);

		if (this.getUsers() == null || this.getUsers().isEmpty())
			this.users = new ArrayList<>();

		TypeReference<ArrayList<Actor>> actorsTypeReference = new TypeReference<>() {
		};
		this.actors = readArrayOfObjectsFromJSON(inputPath + "actors.json", actorsTypeReference);

		if (this.getActors() == null || this.getActors().isEmpty())
			this.actors = new ArrayList<>();

		TypeReference<ArrayList<Request>> requestsTypeReference = new TypeReference<>() {
		};
		this.requests = readArrayOfObjectsFromJSON(inputPath + "requests.json", requestsTypeReference);

		if (this.getRequests() == null || this.getRequests().isEmpty())
			this.requests = new ArrayList<>();

		TypeReference<ArrayList<Production>> productionsTypeReference = new TypeReference<>() {
		};
		this.productions = readArrayOfObjectsFromJSON(inputPath + "production.json", productionsTypeReference);

		if (this.getProductions() == null || this.getProductions().isEmpty())
			this.productions = new ArrayList<>();

		if (this.getUsers() != null && !this.getUsers().isEmpty()) {
			for (User user : this.getUsers()) {
				if (user.getActorsContribution() != null && !user.getActorsContribution().isEmpty())
					for (String actorName : user.getActorsContribution())
						for (Actor actor : this.getActors())
							if (actor.getName().equals(actorName))
								actor.setResponsible(user.getUsername());

				if (user.getProductionsContribution() != null && !user.getProductionsContribution().isEmpty()) {
					for (String productionTitle : user.getProductionsContribution()) {
						for (Production production : this.getProductions()) {
							if (production.getTitle().equals(productionTitle)) {
								production.setResponsible(user.getUsername());
								production.registerObserver((Observer) user);
							}
						}
					}
				}
			}
		}

		if (this.getProductions() != null && !this.getProductions().isEmpty()) {
			for (Production production : this.getProductions()) {
				for (String actorName : production.getActors()) {
					if (this.getActors().stream().noneMatch(actor -> actor.getName().equals(actorName))) {
						Actor newActor = new Actor();
						newActor.setName(actorName);
						this.getActors().add(newActor);
					}
				}

				for (String actorName : production.getActors()) {
					for (Actor actor : this.getActors()) {
						if (actor.getName().equals(actorName)) {
							Actor.Performance performance = new Actor.Performance();
							performance.setTitle(production.getTitle());
							performance.setType(production.getType());
							actor.getPerformances().add(performance);
						}
					}
				}

				for (Rating rating : production.getRatings())
					for (User user : this.getUsers())
						if (user.getUsername().equals(rating.getUsername()))
							user.getReviewedProductions().add(production.getTitle());
			}
		}

		if (this.getRequests() != null && !this.getRequests().isEmpty())
			for (Request request : this.getRequests())
				if (request.getTo().equals("ADMIN"))
					RequestsHolder.addRequest(request);
	}

	public User login() {
		User currentUser = null;
		Credentials credentials = new Credentials();

		Scanner scanner = new Scanner(System.in);

		System.out.print("\033[H\033[2J");
		System.out.flush();
		System.out.println("Enter your credentials:");
		System.out.print("\temail: ");
		credentials.setEmail(scanner.nextLine());

		boolean isEmailValid = false;

		for (User user : this.getUsers()) {
			Credentials userCredentials = user.getInformation().getCredentials();

			if (userCredentials.getEmail().equals(credentials.getEmail())) {
				isEmailValid = true;
				break;
			}
		}

		if (!isEmailValid)
			return null;

		while (currentUser == null) {
			System.out.print("\tpassword: ");
			credentials.setPassword(scanner.nextLine());

			for (User user : this.getUsers()) {
				Credentials userCredentials = user.getInformation().getCredentials();

				if (!userCredentials.getEmail().equals(credentials.getEmail()))
					continue;

				if (!userCredentials.getPassword().equals(credentials.getPassword()))
					break;

				currentUser = UserFactory.createUser(user);
				currentUser.setIsLoggedIn(true);
			}

			if (currentUser == null)
				System.out.println("Wrong password! Try again.");
		}

		return currentUser;
	}

	public ArrayList<AbstractMap.SimpleEntry<String, Set<AccountType>>> instantiateActions() {
		ArrayList<AbstractMap.SimpleEntry<String, Set<AccountType>>> actions = new ArrayList<>();
		ArrayList<String> actionsNames = new ArrayList<>();
		ArrayList<Set<AccountType>> actionsAccountTypes = new ArrayList<>();

		actionsNames.add("View productions details");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		actionsNames.add("View actors details");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		actionsNames.add("View notifications");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		actionsNames.add("Search for actor/movie/series");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		actionsNames.add("Add/Delete actor/movie/series to/from favorites");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		actionsNames.add("Create/Delete request");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor));

		actionsNames.add("Add/Delete actor/movie/series from system");
		actionsAccountTypes.add(EnumSet.of(AccountType.Contributor, AccountType.Admin));

		actionsNames.add("View/Solve request");
		actionsAccountTypes.add(EnumSet.of(AccountType.Contributor, AccountType.Admin));

		actionsNames.add("Update actor/movie/series details");
		actionsAccountTypes.add(EnumSet.of(AccountType.Contributor, AccountType.Admin));

		actionsNames.add("Add/Delete review to/from actor/movie/series");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular));

		actionsNames.add("Add/Delete user from system");
		actionsAccountTypes.add(EnumSet.of(AccountType.Admin));

		actionsNames.add("Logout");
		actionsAccountTypes.add(EnumSet.of(AccountType.Regular, AccountType.Contributor, AccountType.Admin));

		for (int index = 0; index < actionsNames.size(); ++index)
			actions.add(new AbstractMap.SimpleEntry<>(actionsNames.get(index), actionsAccountTypes.get(index)));

		return actions;
	}

	public boolean startApplicationFlow(User currentUser) throws InvalidCommandException {
		Scanner scanner = new Scanner(System.in);

		while (currentUser.getIsLoggedIn()) {
			ArrayList<AbstractMap.SimpleEntry<String, Set<AccountType>>> actions = instantiateActions();
			Map<Integer, Integer> displayedActionIndexToActualIndexMap = new HashMap<>();

			System.out.println("Your experience: " + currentUser.getExperience());
			System.out.println("Choose an action:");

			int displayedIndex = 0;

			for (int index = 0; index < actions.size(); ++index) {
				if (actions.get(index).getValue().contains(currentUser.getUserType())) {
					displayedActionIndexToActualIndexMap.put(++displayedIndex, index);
					System.out.println("\t" + displayedIndex + ") " + actions.get(index).getKey());
				}
			}

			System.out.print("Your choice: ");
			int choice = scanner.hasNextInt() ? scanner.nextInt() : -1;
			scanner.nextLine();
			System.out.println();

			if (choice == -1)
				continue;

			Integer actualActionIndex = displayedActionIndexToActualIndexMap.get(choice);

			if (actualActionIndex == null)
				throw new InvalidCommandException("Invalid command!");

			switch (actualActionIndex) {
				case 0:
					new ViewProductionsAction().execute(currentUser);
					break;

				case 1:
					new ViewActorsAction().execute(currentUser);
					break;

				case 2:
					new ViewNotificationsAction().execute(currentUser);
					break;

				case 3:
					new SearchActorProductionAction().execute(currentUser);
					break;

				case 4:
					new AddDeleteFavoritesAction().execute(currentUser);
					break;

				case 5:
					new CreateDeleteRequestsAction().execute(currentUser);
					break;

				case 6:
					new AddDeleteActorsProductionsAction().execute(currentUser);
					break;

				case 7:
					new ViewSolveRequestsAction().execute(currentUser);
					break;

				case 8:
					new UpdateActorsProductionsAction().execute(currentUser);
					break;

				case 9:
					new AddDeleteReviewsAction().execute(currentUser);
					break;

				case 10:
					new AddDeleteUsersAction().execute(currentUser);
					break;

				case 11:
					return handleLogoutMenu(currentUser);
			}
		}

		return true;
	}

	public boolean handleLogoutMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		currentUser.logout();
		System.out.println("You have been logged out!");

		System.out.println("What do you want to do now?");
		System.out.println("\t1) Login");
		System.out.println("\t2) Exit");

		System.out.print("Your choice: ");
		int choice = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		return choice == 1;
	}
}
