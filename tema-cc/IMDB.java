package org.example;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.example.actor.Actor;
import org.example.enumerations.AccountType;
import org.example.enumerations.Genre;
import org.example.enumerations.RequestStatus;
import org.example.enumerations.RequestType;
import org.example.exceptions.InformationIncompleteException;
import org.example.exceptions.InvalidCommandException;
import org.example.interfaces.Observer;
import org.example.production.Movie;
import org.example.production.Production;
import org.example.production.Rating;
import org.example.production.Series;
import org.example.request.Request;
import org.example.request.RequestsHolder;
import org.example.strategies.ActorAdditionStrategy;
import org.example.strategies.ProductionAdditionStrategy;
import org.example.strategies.RequestApprovalStrategy;
import org.example.strategies.ReviewAdditionStrategy;
import org.example.user.*;
import org.example.utils.Utils;

import java.io.File;
import java.io.IOException;
import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;

public class IMDB {
	private static volatile IMDB imdb = null;

	private ArrayList<User> users;
	private ArrayList<Actor> actors;
	private ArrayList<Request> requests;
	private ArrayList<Production> productions;

	private IMDB() {
		if (imdb != null)
			throw new RuntimeException("Use getInstance() method!");
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

				System.out.println("\nWelcome back, " + currentUser.getUsername() + "!");
				System.out.println("You are logged in as " + currentUser.getUserType() + ".\n");

				isRunning = handleActionsMenu(currentUser);
			}
		} catch (InvalidCommandException | InformationIncompleteException exception) {
			System.out.println(exception.getMessage());
		}
	}

	public void readFromDatabase() {
		String inputPath = "src/main/resources/input/";

		TypeReference<ArrayList<User>> usersTypeReference = new TypeReference<>() {
		};
		users = readArrayOfObjectsFromJSON(inputPath + "accounts.json", usersTypeReference);

		TypeReference<ArrayList<Actor>> actorsTypeReference = new TypeReference<>() {
		};
		actors = readArrayOfObjectsFromJSON(inputPath + "actors.json", actorsTypeReference);

		TypeReference<ArrayList<Request>> requestsTypeReference = new TypeReference<>() {
		};
		requests = readArrayOfObjectsFromJSON(inputPath + "requests.json", requestsTypeReference);

		TypeReference<ArrayList<Production>> productionsTypeReference = new TypeReference<>() {
		};
		productions = readArrayOfObjectsFromJSON(inputPath + "production.json", productionsTypeReference);

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
							break;
						}
					}
				}

				for (Rating rating : production.getRatings()) {
					for (User user : this.getUsers()) {
						if (user.getUsername().equals(rating.getUsername())) {
							user.getReviewedProductions().add(production.getTitle());
							break;
						}
					}
				}
			}
		}

		if (this.getUsers() != null && !this.getUsers().isEmpty()) {
			for (User user : this.getUsers()) {
				if (user.getActorsContribution() != null && !user.getActorsContribution().isEmpty()) {
					for (String actorName : user.getActorsContribution()) {
						for (Actor actor : this.getActors()) {
							if (actor.getName().equals(actorName)) {
								actor.setResponsible(user.getUsername());
								break;
							}
						}
					}
				}

				if (user.getProductionsContribution() != null && !user.getProductionsContribution().isEmpty()) {
					for (String productionTitle : user.getProductionsContribution()) {
						for (Production production : this.getProductions()) {
							if (production.getTitle().equals(productionTitle)) {
								production.setResponsible(user.getUsername());
								production.registerObserver((Observer) user);
								break;
							}
						}
					}
				}
			}
		}

		if (this.getRequests() != null && !this.getRequests().isEmpty())
			for (Request request : this.getRequests())
				if (request.getTo().equals("ADMIN"))
					RequestsHolder.addRequest(request);
	}

	public User login() {
		Scanner scanner = new Scanner(System.in);
		User currentUser = null;
		Credentials credentials = new Credentials();

		System.out.print("\033[H\033[2J");
		System.out.flush();
		System.out.println("Enter your credentials.");
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

			if (currentUser == null) {
				System.out.println("Wrong password! Try again.");
			}
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

	public boolean handleActionsMenu(User currentUser) throws InvalidCommandException, InformationIncompleteException {
		Scanner scanner = new Scanner(System.in);

		while (currentUser.getIsLoggedIn()) {
			System.out.println("Your experience: " + currentUser.getExperience());
			System.out.println("Choose an action:");

			ArrayList<AbstractMap.SimpleEntry<String, Set<AccountType>>> actions = instantiateActions();
			Map<Integer, Integer> displayedActionIndexToActualIndexMap = new HashMap<>();

			int displayedIndex = 0;

			for (int index = 0; index < actions.size(); ++index) {
				if (actions.get(index).getValue().contains(currentUser.getUserType())) {
					displayedActionIndexToActualIndexMap.put(++displayedIndex, index);
					System.out.println("\t" + displayedIndex + ") " + actions.get(index).getKey());
				}
			}

			System.out.print("Your choice: ");
			int userInput = scanner.hasNextInt() ? scanner.nextInt() : -1;
			scanner.nextLine();
			System.out.println();

			if (userInput == -1)
				continue;

			Integer actualActionIndex = displayedActionIndexToActualIndexMap.get(userInput);

			if (actualActionIndex == null)
				throw new InvalidCommandException("Invalid command!");

			switch (actualActionIndex) {
				case 0:
					handleViewProductionsMenu();
					break;

				case 1:
					handleViewActorsMenu();
					break;

				case 2:
					handleViewNotificationsMenu(currentUser);
					break;

				case 3:
					handleSearchMenu();
					break;

				case 4:
					handleAddDeleteFavoritesMenu(currentUser);
					break;

				case 5:
					handleCreateDeleteRequestsMenu(currentUser);
					break;

				case 6:
					handleAddDeleteActorsProductionsMenu(currentUser);
					break;

				case 7:
					handleViewSolveRequestsMenu(currentUser);
					break;

				case 8:
					handleUpdateActorsProductionsMenu(currentUser);
					break;

				case 9:
					handleAddDeleteReviewsMenu(currentUser);
					break;

				case 10:
					handleAddDeleteUsersMenu(currentUser);
					break;

				case 11:
					return handleLogoutMenu(currentUser);
			}
		}

		return true;
	}

	public void handleViewProductionsMenu() {
		ArrayList<Production> productions = new ArrayList<>(this.getProductions());

		Scanner scanner = new Scanner(System.in);

		System.out.println("Do you want to sort the productions?");
		System.out.println("\t1) Yes");
		System.out.println("\t2) No");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		System.out.println("Do you want to filter the productions?");
		System.out.println("\t1) Yes");
		System.out.println("\t2) No");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputTwo = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputTwo < 1 || userInputTwo > 2)
			return;

		if (userInputTwo == 1) {
			System.out.println("What do you want to filter by?");
			System.out.println("\t1) Genre");
			System.out.println("\t2) Number of ratings");
			System.out.println("\t3) Exit");

			System.out.print("Your choice: ");
			int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
			scanner.nextLine();
			System.out.println();

			if (userInputThree < 1 || userInputThree > 2)
				return;

			switch (userInputThree) {
				case 1: {
					System.out.print("Enter the genre: ");
					String userInputFour = scanner.nextLine();
					System.out.println();

					if (!Genre.contains(userInputFour)) {
						System.out.println("Invalid genre!");
						System.out.println();
						break;
					}

					productions = productions.stream().filter(
							production -> production.getGenres().contains(Genre.valueOf(userInputFour))).collect(
							Collectors.toCollection(ArrayList::new));

					break;
				}

				case 2: {
					System.out.print("Enter the number of ratings: ");
					int userInputFour = scanner.hasNextInt() ? scanner.nextInt() : -1;
					scanner.nextLine();
					System.out.println();

					if (userInputFour < 1) {
						System.out.println("Invalid number of ratings!");
						System.out.println();
						break;
					}

					productions = productions.stream().filter(
							production -> production.getRatings().size() >= userInputFour).collect(
							Collectors.toCollection(ArrayList::new));

					break;
				}
			}
		}

		if (userInputOne == 1)
			productions.sort(null);

		System.out.println("Productions:");

		for (Production production : productions)
			production.displayInfo();

	}

	public void handleViewActorsMenu() {
		ArrayList<Actor> actors = new ArrayList<>(this.getActors());

		Scanner scanner = new Scanner(System.in);

		System.out.println("Do you want to sort the actors?");
		System.out.println("\t1) Yes");
		System.out.println("\t2) No");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		if (userInputOne == 1)
			actors.sort(null);

		System.out.println("Actors:");

		for (Actor actor : actors)
			actor.displayInfo();
	}

	public void handleViewNotificationsMenu(User currentUser) {
		System.out.println("Notifications:");

		if (currentUser.getNotifications() == null || currentUser.getNotifications().isEmpty())
			System.out.println("You have no notifications!");
		else
			for (String notification : currentUser.getNotifications())
				System.out.println((currentUser.getNotifications().indexOf(notification) + 1) + ") " + notification);

		System.out.println();
	}

	public void handleSearchMenu() {
		Scanner scanner = new Scanner(System.in);

		System.out.println("What are you searching for?");
		System.out.println("\t1) Actor");
		System.out.println("\t2) Movie");
		System.out.println("\t3) Series");
		System.out.println("\t4) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 3)
			return;

		System.out.print("Enter the name: ");
		String userInputTwo = scanner.nextLine();
		System.out.println();

		switch (userInputOne) {
			case 1:
				for (Actor actor : this.getActors()) {
					if (actor.getName().equals(userInputTwo)) {
						actor.displayInfo();
						break;
					}
				}

				System.out.println("Actor not found!");
				System.out.println();
				break;

			case 2:
				for (Production production : this.getProductions()) {
					if (production.getType().equals("Movie") && production.getTitle().equals(userInputTwo)) {
						production.displayInfo();
						break;
					}
				}

				System.out.println("Movie not found!");
				System.out.println();
				break;

			case 3:
				for (Production production : this.getProductions()) {
					if (production.getType().equals("Series") && production.getTitle().equals(userInputTwo)) {
						production.displayInfo();
						break;
					}
				}

				System.out.println("Series not found!");
				System.out.println();
				break;
		}
	}

	public void handleAddDeleteFavoritesMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		System.out.println("Favorites:");
		System.out.println("\tActors: " + currentUser.getFavoriteActors());
		System.out.println("\tProductions: " + currentUser.getFavoriteProductions());

		System.out.println("What do you want to do?");
		System.out.println("\t1) Add");
		System.out.println("\t2) Delete");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		System.out.println("What do you want to " + (userInputOne == 1 ? "add" : "delete") + "?");
		System.out.println("\t1) Actor");
		System.out.println("\t2) Production");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputTwo = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputTwo < 1 || userInputTwo > 2)
			return;

		System.out.print("Enter the name: ");
		String userInputThree = scanner.nextLine();
		System.out.println();

		switch (userInputTwo) {
			case 1:
				for (Actor actor : this.getActors()) {
					if (actor.getName().equals(userInputThree)) {
						if (userInputOne == 1) {
							currentUser.addActorToFavorites(actor);
							System.out.println();
							break;
						}

						currentUser.removeActorFromFavorites(actor);
						System.out.println();
						break;
					}
				}

				System.out.println("Actor not found!");
				System.out.println();
				break;

			case 2:
				for (Production production : this.getProductions()) {
					if (production.getTitle().equals(userInputThree)) {
						if (userInputOne == 1) {
							currentUser.addProductionToFavorites(production);
							System.out.println();
							break;
						}

						currentUser.removeProductionFromFavorites(production);
						System.out.println();
						break;
					}
				}

				System.out.println("Production not found!");
				System.out.println();
				break;
		}
	}

	public void handleCreateDeleteRequestsMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		System.out.println("What do you want to do?");
		System.out.println("\t1) Create");
		System.out.println("\t2) Delete");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		switch (userInputOne) {
			case 1: {
				System.out.println("What type of request do you want to create?");
				System.out.println("\t1) Delete account");
				System.out.println("\t2) Actor issue");
				System.out.println("\t3) Production issue");
				System.out.println("\t4) Others");
				System.out.println("\t5) Exit");

				System.out.print("Your choice: ");
				int userInputTwo = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputTwo < 1 || userInputTwo > 4)
					break;

				System.out.print("Enter the reason: ");
				String userInputThree = scanner.nextLine();
				System.out.println();

				String userInputFour;
				if (userInputTwo == 2 || userInputTwo == 3) {
					System.out.print("Enter the name: ");
					userInputFour = scanner.nextLine();
					System.out.println();
				} else {
					userInputFour = null;
				}

				Request newRequest = new Request();
				newRequest.setCreatedDate(LocalDateTime.now());
				newRequest.setUsername(currentUser.getUsername());
				newRequest.setDescription(userInputThree);

				switch (userInputTwo) {
					case 1:
						newRequest.setType(RequestType.DELETE_ACCOUNT);
						newRequest.setTo("ADMIN");
						((Regular) currentUser).createRequest(newRequest);
						break;

					case 2:
						newRequest.setType(RequestType.ACTOR_ISSUE);

						Actor actor = this.getActors().stream().filter(a -> a.getName().equals(userInputFour)).findFirst().orElse(
								null);

						if (actor == null) {
							System.out.println("Actor not found!");
							System.out.println();
							break;
						}

						if (actor.getResponsible().equals(currentUser.getUsername())) {
							System.out.println("You are already responsible for this actor!");
							System.out.println();
							break;
						}

						newRequest.setActorName(userInputFour);
						newRequest.setTo(actor.getResponsible());

						this.getUsers().stream().filter(
								user -> user.getUsername().equals(actor.getResponsible())).findFirst().ifPresent(
								responsibleUser -> responsibleUser.getNotifications().add(
										"Someone just made a request that you are responsible for!"));

						break;

					case 3:
						newRequest.setType(RequestType.MOVIE_ISSUE);

						Production production = this.getProductions().stream().filter(
								p -> p.getTitle().equals(userInputFour)).findFirst().orElse(null);

						if (production == null) {
							System.out.println("Production not found!");
							System.out.println();
							break;
						}

						if (production.getResponsible().equals(currentUser.getUsername())) {
							System.out.println("You are already responsible for this production!");
							System.out.println();
							break;
						}

						newRequest.setMovieTitle(userInputFour);
						newRequest.setTo(production.getResponsible());

						this.getUsers().stream().filter(
								user -> user.getUsername().equals(production.getResponsible())).findFirst().ifPresent(
								responsibleUser -> responsibleUser.getNotifications().add(
										"Someone just made a request that you are responsible for!"));

						break;

					case 4:
						newRequest.setType(RequestType.OTHERS);
						newRequest.setTo("ADMIN");
						((Regular) currentUser).createRequest(newRequest);
						break;
				}

				newRequest.registerObserver((Observer) currentUser);
				this.getRequests().add(newRequest);
				break;
			}

			case 2: {
				ArrayList<Request> userRequests = this.getRequests().stream().filter(
						request -> request.getUsername().equals(currentUser.getUsername())).collect(
						Collectors.toCollection(ArrayList::new));

				if (userRequests.isEmpty()) {
					System.out.println("You have no requests!");
					System.out.println();
					break;
				}

				System.out.println("Requests: " + userRequests);
				System.out.println();

				System.out.print("Enter the id: ");
				String userInputTwo = scanner.nextLine();
				System.out.println();

				Request request = userRequests.stream().filter(r -> r.getId().equals(userInputTwo)).findFirst().orElse(null);

				if (request == null) {
					System.out.println("Request not found!");
					System.out.println();
					break;
				}

				this.getRequests().remove(request);
				if (request.getTo().equals("ADMIN"))
					((Regular) currentUser).removeRequest(request);

				break;
			}
		}
	}

	public void handleAddDeleteActorsProductionsMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		System.out.println("What do you want to do?");
		System.out.println("\t1) Add");
		System.out.println("\t2) Delete");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		System.out.println("What do you want to " + (userInputOne == 1 ? "add" : "delete") + "?");
		System.out.println("\t1) Actor");
		System.out.println("\t2) Movie");
		System.out.println("\t3) Series");
		System.out.println("\t4) Exit");

		System.out.print("Your choice: ");
		int userInputTwo = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputTwo < 1 || userInputTwo > 3)
			return;

		if (userInputOne == 2) {
			if (userInputTwo == 1) {
				System.out.print("Actors: [");

				for (Actor actor : this.getActors()) {
					System.out.print(actor.getName());
					if (actor != this.getActors().get(this.getActors().size() - 1))
						System.out.print(", ");
				}

				System.out.println("]");
				System.out.println();
			}

			if (userInputTwo == 2 || userInputTwo == 3) {
				System.out.print("Productions: [");

				for (Production production : this.getProductions()) {
					System.out.print(production.getTitle());
					if (production != this.getProductions().get(this.getProductions().size() - 1))
						System.out.print(", ");
				}

				System.out.println("]");
				System.out.println();
			}
		}

		System.out.print("Enter the name: ");
		String userInputThree = scanner.nextLine();
		System.out.println();

		switch (userInputTwo) {
			case 1: {
				if (userInputOne == 1) {
					Actor newActor = new Actor();
					newActor.setName(userInputThree);
					newActor.setResponsible(currentUser.getUsername());
					((Staff) currentUser).addActorSystem(newActor);
					currentUser.updateExperience(new ActorAdditionStrategy());
					break;
				}

				Actor actor = this.getActors().stream().filter(a -> a.getName().equals(userInputThree)).findFirst().orElse(
						null);

				if (actor == null) {
					System.out.println("Actor not found!");
					System.out.println();
					break;
				}

				if (!actor.getResponsible().equals("ADMIN") && !actor.getResponsible().equals(currentUser.getUsername())) {
					System.out.println("You are not responsible for this actor!");
					System.out.println();
					break;
				}

				if (actor.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
					System.out.println("You are not an admin!");
					System.out.println();
					break;
				}

				((Staff) currentUser).removeActorSystem(userInputThree);
				break;
			}

			case 2: {
				if (userInputOne == 1) {
					Movie newMovie = new Movie();
					newMovie.setTitle(userInputThree);
					newMovie.setResponsible(currentUser.getUsername());
					newMovie.registerObserver((Observer) currentUser);
					((Staff) currentUser).addProductionSystem(newMovie);
					currentUser.updateExperience(new ProductionAdditionStrategy());
					break;
				}

				Production production = this.getProductions().stream().filter(
						p -> p.getType().equals("Movie") && p.getTitle().equals(userInputThree)).findFirst().orElse(null);

				if (production == null) {
					System.out.println("Movie not found!");
					System.out.println();
					break;
				}

				if (!production.getResponsible().equals(currentUser.getUsername())) {
					System.out.println("You are not responsible for this movie!");
					System.out.println();
					break;
				}

				if (production.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
					System.out.println("You are not an admin!");
					System.out.println();
					break;
				}

				((Staff) currentUser).removeProductionSystem(userInputThree);
				break;
			}

			case 3: {
				if (userInputOne == 1) {
					Series newSeries = new Series();
					newSeries.setTitle(userInputThree);
					newSeries.setResponsible(currentUser.getUsername());
					newSeries.registerObserver((Observer) currentUser);
					((Staff) currentUser).addProductionSystem(newSeries);
					currentUser.updateExperience(new ProductionAdditionStrategy());
					break;
				}

				Production production = this.getProductions().stream().filter(
						p -> p.getType().equals("Series") && p.getTitle().equals(userInputThree)).findFirst().orElse(null);

				if (production == null) {
					System.out.println("Series not found!");
					System.out.println();
					break;
				}

				if (!production.getResponsible().equals(currentUser.getUsername())) {
					System.out.println("You are not responsible for this series!");
					System.out.println();
					break;
				}

				if (production.getResponsible().equals("ADMIN") && !currentUser.getUserType().equals(AccountType.Admin)) {
					System.out.println("You are not an admin!");
					System.out.println();
					break;
				}

				((Staff) currentUser).removeProductionSystem(userInputThree);
				break;
			}
		}
	}

	public void handleViewSolveRequestsMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		System.out.println("What do you want to do?");
		System.out.println("\t1) View");
		System.out.println("\t2) Solve");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		ArrayList<Request> adminRequests = RequestsHolder.getRequests();
		System.out.println("Admin requests: " + adminRequests);
		System.out.println();

		ArrayList<Request> responsibleRequests = this.getRequests().stream().filter(
				request -> request.getTo().equals(currentUser.getUsername())).collect(Collectors.toCollection(ArrayList::new));
		System.out.println("Requests you are responsible for: " + responsibleRequests);
		System.out.println();

		switch (userInputOne) {
			case 1:
				break;

			case 2: {
				System.out.print("Enter the id: ");
				String userInputTwo = scanner.nextLine();
				System.out.println();

				Request request = responsibleRequests.stream().filter(r -> r.getId().equals(userInputTwo)).findFirst().orElse(
						null);

				if (request == null)
					request = adminRequests.stream().filter(r -> r.getId().equals(userInputTwo)).findFirst().orElse(null);

				if (request == null) {
					System.out.println("Request not found!");
					System.out.println();
					break;
				}

				System.out.println("What do you want to do?");
				System.out.println("\t1) Approve");
				System.out.println("\t2) Reject");
				System.out.println("\t3) Exit");

				System.out.print("Your choice: ");
				int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputThree < 1 || userInputThree > 2)
					break;

				if (userInputThree == 1) {
					request.setStatus(RequestStatus.APPROVED);

					if (request.getType().equals(RequestType.ACTOR_ISSUE) || request.getType().equals(RequestType.MOVIE_ISSUE)) {
						Request finalRequest = request;
						this.getUsers().stream().filter(
								user -> user.getUsername().equals(finalRequest.getUsername())).findFirst().ifPresent(
								creatorUser -> creatorUser.updateExperience(new RequestApprovalStrategy()));
					}
				}

				if (userInputThree == 2)
					request.setStatus(RequestStatus.REJECTED);

				request.notifyObservers();

				this.getRequests().remove(request);
				if (request.getTo().equals("ADMIN"))
					RequestsHolder.removeRequest(request);

				break;
			}
		}
	}

	public void handleUpdateActorsProductionsMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		System.out.println("What do you want to update?");
		System.out.println("\t1) Actor");
		System.out.println("\t2) Movie");
		System.out.println("\t3) Series");
		System.out.println("\t4) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 3)
			return;

		System.out.print("Enter the name: ");
		String userInputTwo = scanner.nextLine();
		System.out.println();

		switch (userInputOne) {
			case 1: {
				Actor updatedActor = null;

				for (Actor actor : this.getActors()) {
					if (actor.getName().equals(userInputTwo)) {
						updatedActor = actor;
						break;
					}
				}

				if (updatedActor == null) {
					System.out.println("Actor not found!");
					System.out.println();
					break;
				}

				System.out.println("What do you want to update?");
				System.out.println("\t1) Name");
				System.out.println("\t2) Biography");
				System.out.println("\t3) Exit");

				System.out.print("Your choice: ");
				int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputThree < 1 || userInputThree > 2)
					break;

				switch (userInputThree) {
					case 1:
						System.out.print("Enter the new name: ");
						String userInputFour = scanner.nextLine();
						System.out.println();
						updatedActor.setName(userInputFour);
						break;

					case 2:
						System.out.print("Enter the new description: ");
						String userInputFive = scanner.nextLine();
						System.out.println();
						updatedActor.setBiography(userInputFive);
						break;
				}

				((Staff) currentUser).updateActorSystem(updatedActor);
				break;
			}

			case 2: {
				Movie updatedMovie = null;

				for (Production production : this.getProductions()) {
					if (production.getType().equals("Movie") && production.getTitle().equals(userInputTwo)) {
						updatedMovie = (Movie) production;
						break;
					}
				}

				if (updatedMovie == null) {
					System.out.println("Movie not found!");
					System.out.println();
					break;
				}

				System.out.println("What do you want to update?");
				System.out.println("\t1) Title");
				System.out.println("\t2) Plot");
				System.out.println("\t3) Exit");

				System.out.print("Your choice: ");
				int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputThree < 1 || userInputThree > 2)
					break;

				switch (userInputThree) {
					case 1:
						System.out.print("Enter the new title: ");
						String userInputFour = scanner.nextLine();
						System.out.println();
						updatedMovie.setTitle(userInputFour);
						break;

					case 2:
						System.out.print("Enter the new plot: ");
						String userInputNine = scanner.nextLine();
						System.out.println();
						updatedMovie.setPlot(userInputNine);
						break;
				}

				((Staff) currentUser).updateProductionSystem(updatedMovie);
				break;
			}

			case 3:
				Series updatedSeries = null;

				for (Production production : this.getProductions()) {
					if (production.getType().equals("Series") && production.getTitle().equals(userInputTwo)) {
						updatedSeries = (Series) production;
						break;
					}
				}

				if (updatedSeries == null) {
					System.out.println("Series not found!");
					System.out.println();
					break;
				}

				System.out.println("What do you want to update?");
				System.out.println("\t1) Title");
				System.out.println("\t2) Plot");
				System.out.println("\t3) Exit");

				System.out.print("Your choice: ");
				int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputThree < 1 || userInputThree > 2)
					break;

				switch (userInputThree) {
					case 1:
						System.out.print("Enter the new title: ");
						String userInputFour = scanner.nextLine();
						System.out.println();
						updatedSeries.setTitle(userInputFour);
						break;

					case 2:
						System.out.print("Enter the new plot: ");
						String userInputNine = scanner.nextLine();
						System.out.println();
						updatedSeries.setPlot(userInputNine);
						break;
				}

				((Staff) currentUser).updateProductionSystem(updatedSeries);
				break;
		}
	}

	public void handleAddDeleteReviewsMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		ArrayList<Rating> ratings = new ArrayList<>();

		for (Production production : this.getProductions())
			ratings.addAll(production.getRatings().stream().filter(
					rating -> rating.getUsername().equals(currentUser.getUsername())).collect(
					Collectors.toCollection(ArrayList::new)));

		System.out.println("Your ratings: " + ratings);
		System.out.println();

		System.out.println("What do you want to do?");
		System.out.println("\t1) Add");
		System.out.println("\t2) Delete");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		System.out.print("Enter the name: ");
		String userInputTwo = scanner.nextLine();
		System.out.println();

		switch (userInputOne) {
			case 1:
				if (this.getProductions().stream().noneMatch(production -> production.getTitle().equals(userInputTwo))) {
					System.out.println("Production not found!");
					System.out.println();
					break;
				}

				if (ratings.stream().anyMatch(rating -> rating.getProductionTitle().equals(userInputTwo))) {
					System.out.println("You have already reviewed this production!");
					System.out.println();
					break;
				}

				System.out.print("Enter the rating: ");
				int userInputThree = scanner.hasNextInt() ? scanner.nextInt() : -1;
				scanner.nextLine();
				System.out.println();

				if (userInputThree < 1 || userInputThree > 10) {
					System.out.println("Invalid rating!");
					System.out.println();
					break;
				}

				System.out.print("Enter the review: ");
				String userInputFour = scanner.nextLine();
				System.out.println();

				for (Production production : this.getProductions()) {
					if (production.getTitle().equals(userInputTwo)) {
						Rating newRating = new Rating();
						newRating.setUsername(currentUser.getUsername());
						newRating.setRating(userInputThree);
						newRating.setComment(userInputFour);
						newRating.setProductionTitle(userInputTwo);
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
				if (ratings.stream().noneMatch(rating -> rating.getProductionTitle().equals(userInputTwo))) {
					System.out.println("You have not reviewed this production!");
					System.out.println();
					break;
				}

				for (Production production : this.getProductions())
					if (production.getTitle().equals(userInputTwo))
						production.getRatings().removeIf(rating -> rating.getUsername().equals(currentUser.getUsername()));

				break;
		}
	}

	public void handleAddDeleteUsersMenu(User currentUser) throws InformationIncompleteException {
		Scanner scanner = new Scanner(System.in);

		System.out.print("Users: [");

		for (User user : this.getUsers()) {
			System.out.print(user.getUsername());
			if (user != this.getUsers().get(this.getUsers().size() - 1))
				System.out.print(", ");
		}

		System.out.println("]");
		System.out.println();

		System.out.println("What do you want to do?");
		System.out.println("\t1) Add");
		System.out.println("\t2) Delete");
		System.out.println("\t3) Exit");

		System.out.print("Your choice: ");
		int userInputOne = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		if (userInputOne < 1 || userInputOne > 2)
			return;

		System.out.print("Enter the " + (userInputOne == 1 ? "name" : "username") + ": ");
		String userInputTwo = scanner.nextLine();
		System.out.println();

		switch (userInputOne) {
			case 1:
				String firstName = userInputTwo.split(" ")[0];
				String lastName = userInputTwo.split(" ")[1];
				String username = Utils.generateUsername(firstName, lastName);
				String email = Utils.generateEmail(firstName, lastName);
				String password = Utils.generateStrongPassword(12);

				User newUser = UserFactory.createUser(AccountType.Regular);

				if (email.isEmpty() || password.isEmpty() || userInputTwo.isEmpty())
					throw new InformationIncompleteException("Information is incomplete!");

				User.Information.InformationBuilder informationBuilder =
						new User.Information.InformationBuilder().setCredentials(
						new Credentials(email, password)).setName(userInputTwo);

				newUser.setUsername(username);
				newUser.setInformation(informationBuilder.build());

				this.getUsers().add(newUser);

				System.out.println("The credentials are:");
				System.out.println("\temail: " + email);
				System.out.println("\tpassword: " + password);

				break;

			case 2:
				if (userInputTwo.equals(currentUser.getUsername())) {
					System.out.println("You cannot delete yourself!");
					System.out.println();
					break;
				}

				User user =
						this.getUsers().stream().filter(u -> u.getUsername().equals(userInputTwo)).findFirst().orElse(null);

				if (user == null) {
					System.out.println("User not found!");
					System.out.println();
					break;
				}

				if (user.getUserType() == AccountType.Regular)
					for (Production production : this.getProductions())
						production.getRatings().removeIf(rating -> rating.getUsername().equals(user.getUsername()));

				if (user.getUserType() == AccountType.Contributor) {
					for (Actor actor : this.getActors())
						if (actor.getResponsible().equals(user.getUsername()))
							actor.setResponsible("ADMIN");

					for (Production production : this.getProductions())
						if (production.getResponsible().equals(user.getUsername()))
							production.setResponsible("ADMIN");
				}

				if (user.getUserType() == AccountType.Admin) {
					System.out.println("You cannot delete an admin!");
					System.out.println();
					break;
				}

				this.getRequests().removeIf(request -> request.getUsername().equals(user.getUsername()));

				for (Production production : this.getProductions())
					production.removeObserver((Observer) user);

				for (Request request : this.getRequests())
					request.removeObserver((Observer) user);

				this.getUsers().remove(user);
				break;
		}
	}

	public boolean handleLogoutMenu(User currentUser) {
		Scanner scanner = new Scanner(System.in);

		currentUser.logout();
		System.out.println("You have been logged out!");

		System.out.println("What do you want to do now?");
		System.out.println("\t1) Login");
		System.out.println("\t2) Exit");

		System.out.print("Your choice: ");
		int userInput = scanner.hasNextInt() ? scanner.nextInt() : -1;
		scanner.nextLine();
		System.out.println();

		return userInput == 1;
	}
}
