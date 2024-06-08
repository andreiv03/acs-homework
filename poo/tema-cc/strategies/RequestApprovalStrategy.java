package org.example.strategies;

import org.example.interfaces.ExperienceStrategy;

public class RequestApprovalStrategy implements ExperienceStrategy {
	@Override
	public int calculateExperience() {
		return 25;
	}
}
