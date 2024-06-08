package org.example.actions;

import org.example.IMDB;
import org.example.enumerations.RequestStatus;
import org.example.enumerations.RequestType;
import org.example.exceptions.InvalidOptionException;
import org.example.request.Request;
import org.example.request.RequestsHolder;
import org.example.strategies.RequestApprovalStrategy;
import org.example.user.User;

import java.util.ArrayList;
import java.util.stream.Collectors;

public class ViewSolveRequestsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();

			System.out.println("What do you want to do?");
			System.out.println("\t1) View");
			System.out.println("\t2) Solve");
			System.out.println("\t3) Exit");

			int action = getIntegerUserInput(3);

			if (action == 3)
				return;

			ArrayList<Request> adminRequests = RequestsHolder.getRequests();
			System.out.println("Admin requests: " + adminRequests);
			System.out.println();

			ArrayList<Request> responsibleRequests = imdb.getRequests().stream().filter(
					request -> request.getTo().equals(currentUser.getUsername())).collect(
					Collectors.toCollection(ArrayList::new));
			System.out.println("Requests you are responsible for: " + responsibleRequests);
			System.out.println();

			switch (action) {
				case 1:
					break;

				case 2:
					String id = getStringUserInput("Enter request id: ");
					Request request =
							responsibleRequests.stream().filter(r -> r.getId().equals(id)).findFirst().orElse(null);

					if (request == null)
						request = adminRequests.stream().filter(r -> r.getId().equals(id)).findFirst().orElse(null);

					if (request == null) {
						System.out.println("Request not found!");
						System.out.println();
						return;
					}

					System.out.println("What do you want to do?");
					System.out.println("\t1) Approve");
					System.out.println("\t2) Reject");
					System.out.println("\t3) Exit");

					int actionTwo = getIntegerUserInput(3);

					if (actionTwo == 3)
						return;

					if (actionTwo == 1) {
						request.setStatus(RequestStatus.APPROVED);

						if (request.getType().equals(RequestType.ACTOR_ISSUE) || request.getType().equals(
								RequestType.MOVIE_ISSUE)) {
							Request finalRequest = request;
							imdb.getUsers().stream().filter(
									user -> user.getUsername().equals(finalRequest.getUsername())).findFirst().ifPresent(
									creatorUser -> creatorUser.updateExperience(new RequestApprovalStrategy()));
						}
					}

					if (actionTwo == 2)
						request.setStatus(RequestStatus.REJECTED);

					request.notifyObservers();
					imdb.getRequests().remove(request);

					if (request.getTo().equals("ADMIN"))
						RequestsHolder.removeRequest(request);
			}
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
