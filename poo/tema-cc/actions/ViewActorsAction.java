package org.example.actions;

import org.example.IMDB;
import org.example.actor.Actor;
import org.example.exceptions.InvalidOptionException;
import org.example.user.User;

import java.util.ArrayList;

public class ViewActorsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			IMDB imdb = IMDB.getInstance();
			ArrayList<Actor> actors = new ArrayList<>(imdb.getActors());

			System.out.println("Do you want to sort the actors?");
			System.out.println("\t1) Yes");
			System.out.println("\t2) No");
			System.out.println("\t3) Exit");

			int canSortActors = getIntegerUserInput(3);

			if (canSortActors == 3)
				return;

			if (canSortActors == 1)
				actors.sort(null);

			System.out.println("Actors:");

			for (Actor actor : actors)
				actor.displayInfo();
		} catch (InvalidOptionException exception) {
			System.out.println(exception.getMessage());
			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
