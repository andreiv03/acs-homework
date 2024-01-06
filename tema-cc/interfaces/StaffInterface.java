package org.example.interfaces;

import org.example.actor.Actor;
import org.example.production.Production;

public interface StaffInterface {
	void addProductionSystem(Production production);

	void addActorSystem(Actor actor);

	void removeProductionSystem(String title);

	void removeActorSystem(String name);

	void updateProductionSystem(Production production);

	void updateActorSystem(Actor actor);
}
