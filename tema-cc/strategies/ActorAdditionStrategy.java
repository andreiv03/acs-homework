package org.example.strategies;

import org.example.interfaces.ExperienceStrategy;

public class ActorAdditionStrategy implements ExperienceStrategy {
	@Override
	public int calculateExperience() {
		return 10;
	}
}
