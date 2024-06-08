package org.example.user;

import org.example.IMDB;
import org.example.actor.Actor;
import org.example.interfaces.StaffInterface;
import org.example.production.Production;

import java.util.Optional;

public class Staff extends User implements StaffInterface {
	public Staff() {
		super();
	}

	public Staff(User user) {
		super(user);
	}

	@Override
	public void addProductionSystem(Production production) {
		IMDB.getInstance().getProductions().add(production);
	}

	@Override
	public void addActorSystem(Actor actor) {
		IMDB.getInstance().getActors().add(actor);
	}

	@Override
	public void removeProductionSystem(String title) {
		IMDB.getInstance().getProductions().removeIf(production -> production.getTitle().equals(title));
	}

	@Override
	public void removeActorSystem(String name) {
		IMDB.getInstance().getActors().removeIf(actor -> actor.getName().equals(name));
	}

	@Override
	public void updateProductionSystem(Production production) {
		IMDB imdb = IMDB.getInstance();

		Optional<Production> productionOptional = imdb.getProductions().stream().filter(
				p -> p.getTitle().equals(production.getTitle())).findFirst();

		productionOptional.ifPresent(p -> {
			int index = imdb.getProductions().indexOf(p);
			imdb.getProductions().set(index, production);
		});
	}

	@Override
	public void updateActorSystem(Actor actor) {
		IMDB imdb = IMDB.getInstance();

		Optional<Actor> actorOptional = imdb.getActors().stream().filter(
				a -> a.getName().equals(actor.getName())).findFirst();

		actorOptional.ifPresent(a -> {
			int index = imdb.getActors().indexOf(a);
			imdb.getActors().set(index, actor);
		});
	}
}
