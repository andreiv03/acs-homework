package org.example.strategies;

import org.example.interfaces.ExperienceStrategy;

public class ProductionAdditionStrategy implements ExperienceStrategy {
	@Override
	public int calculateExperience() {
		return 15;
	}
}
