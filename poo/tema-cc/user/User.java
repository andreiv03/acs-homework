package org.example.user;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.example.actor.Actor;
import org.example.enumerations.AccountType;
import org.example.interfaces.ExperienceStrategy;
import org.example.production.Production;

import java.time.LocalDate;
import java.util.ArrayList;

// @formatter:off
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.EXISTING_PROPERTY, property = "userType")
@JsonSubTypes({
		@JsonSubTypes.Type(value = Regular.class, name = "Regular"),
		@JsonSubTypes.Type(value = Contributor.class, name = "Contributor"),
		@JsonSubTypes.Type(value = Admin.class, name = "Admin")
})
// @formatter:on
public abstract class User {
	private String username;
	private String experience;
	private Information information;
	private AccountType userType;
	private ArrayList<String> productionsContribution;
	private ArrayList<String> actorsContribution;
	private ArrayList<String> favoriteProductions;
	private ArrayList<String> favoriteActors;
	private boolean isLoggedIn;
	private ArrayList<String> notifications;
	private ArrayList<String> reviewedProductions;

	public User() {
		this.username = null;
		this.experience = null;
		this.information = null;
		this.userType = null;
		this.productionsContribution = null;
		this.actorsContribution = null;
		this.favoriteProductions = null;
		this.favoriteActors = null;
		this.isLoggedIn = false;
		this.notifications = new ArrayList<>();
		this.reviewedProductions = new ArrayList<>();
	}

	public User(String username, String experience, Information information, AccountType userType,
	            ArrayList<String> productionsContribution, ArrayList<String> actorsContribution,
	            ArrayList<String> favoriteProductions, ArrayList<String> favoriteActors) {
		this.username = username;
		this.experience = experience;
		this.information = information;
		this.userType = userType;
		this.productionsContribution = productionsContribution;
		this.actorsContribution = actorsContribution;
		this.favoriteProductions = favoriteProductions;
		this.favoriteActors = favoriteActors;
		this.isLoggedIn = false;
		this.notifications = new ArrayList<>();
		this.reviewedProductions = new ArrayList<>();
	}

	public User(User user) {
		this.username = user.username;
		this.experience = user.experience;
		this.information = user.information;
		this.userType = user.userType;
		this.productionsContribution = user.productionsContribution;
		this.actorsContribution = user.actorsContribution;
		this.favoriteProductions = user.favoriteProductions;
		this.favoriteActors = user.favoriteActors;
		this.isLoggedIn = user.isLoggedIn;
		this.notifications = user.notifications;
		this.reviewedProductions = user.reviewedProductions;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getExperience() {
		if (this.getUserType().equals(AccountType.Admin))
			this.setExperience("INFINITE");

		if (experience == null)
			this.setExperience("0");

		return experience;
	}

	public void setExperience(String experience) {
		this.experience = experience;
	}

	public void updateExperience(ExperienceStrategy experienceStrategy) {
		if (this.getUserType().equals(AccountType.Admin))
			return;

		this.experience = String.valueOf(Integer.parseInt(this.experience) + experienceStrategy.calculateExperience());
	}

	public Information getInformation() {
		return information;
	}

	public void setInformation(Information information) {
		this.information = information;
	}

	public AccountType getUserType() {
		return userType;
	}

	public void setUserType(AccountType userType) {
		this.userType = userType;
	}

	public ArrayList<String> getProductionsContribution() {
		return productionsContribution;
	}

	public void setProductionsContribution(ArrayList<String> productionsContribution) {
		this.productionsContribution = productionsContribution;
	}

	public ArrayList<String> getActorsContribution() {
		return actorsContribution;
	}

	public void setActorsContribution(ArrayList<String> actorsContribution) {
		this.actorsContribution = actorsContribution;
	}

	public ArrayList<String> getFavoriteProductions() {
		return favoriteProductions;
	}

	public void setFavoriteProductions(ArrayList<String> favoriteProductions) {
		this.favoriteProductions = favoriteProductions;
	}

	public void addProductionToFavorites(Production production) {
		favoriteProductions.add(production.getTitle());
	}

	public void removeProductionFromFavorites(Production production) {
		favoriteProductions.remove(production.getTitle());
	}

	public ArrayList<String> getFavoriteActors() {
		return favoriteActors;
	}

	public void setFavoriteActors(ArrayList<String> favoriteActors) {
		this.favoriteActors = favoriteActors;
	}

	public void addActorToFavorites(Actor actor) {
		favoriteActors.add(actor.getName());
	}

	public void removeActorFromFavorites(Actor actor) {
		favoriteActors.remove(actor.getName());
	}

	public boolean getIsLoggedIn() {
		return isLoggedIn;
	}

	public void setIsLoggedIn(boolean isLoggedIn) {
		this.isLoggedIn = isLoggedIn;
	}

	public ArrayList<String> getNotifications() {
		return notifications;
	}

	public void setNotifications(ArrayList<String> notifications) {
		this.notifications = notifications;
	}

	public ArrayList<String> getReviewedProductions() {
		return reviewedProductions;
	}

	public void setReviewedProductions(ArrayList<String> reviewedProductions) {
		this.reviewedProductions = reviewedProductions;
	}

	public void logout() {
		setIsLoggedIn(false);
	}

	public static class Information {
		private final Credentials credentials;
		private final String name;
		private final String country;
		private final int age;
		private final String gender;
		private final LocalDate birthDate;

		public Information() {
			this.credentials = null;
			this.name = null;
			this.country = null;
			this.age = 0;
			this.gender = null;
			this.birthDate = null;
		}

		private Information(InformationBuilder informationBuilder) {
			this.credentials = informationBuilder.credentials;
			this.name = informationBuilder.name;
			this.country = informationBuilder.country;
			this.age = informationBuilder.age;
			this.gender = informationBuilder.gender;
			this.birthDate = informationBuilder.birthDate;
		}

		public Credentials getCredentials() {
			return credentials;
		}

		public String getName() {
			return name;
		}

		public String getCountry() {
			return country;
		}

		public int getAge() {
			return age;
		}

		public String getGender() {
			return gender;
		}

		public LocalDate getBirthDate() {
			return birthDate;
		}

		public static class InformationBuilder {
			private Credentials credentials;
			private String name;
			private String country;
			private int age;
			private String gender;
			private LocalDate birthDate;

			public Information build() {
				return new Information(this);
			}

			public InformationBuilder setCredentials(Credentials credentials) {
				this.credentials = credentials;
				return this;
			}

			public InformationBuilder setName(String name) {
				this.name = name;
				return this;
			}

			public InformationBuilder setCountry(String country) {
				this.country = country;
				return this;
			}

			public InformationBuilder setAge(int age) {
				this.age = age;
				return this;
			}

			public InformationBuilder setGender(String gender) {
				this.gender = gender;
				return this;
			}

			public InformationBuilder setBirthDate(LocalDate birthDate) {
				this.birthDate = birthDate;
				return this;
			}
		}
	}
}
