package org.poo.cb.model;

import com.fasterxml.jackson.annotation.JsonIgnore;

import java.util.ArrayList;

public class User {
	private final String email;
	private final String firstName;
	private final String lastName;
	private final String address;
	private final Portfolio portfolio;
	private final ArrayList<String> friends;
	private boolean hasPremium;

	public User(UserBuilder userBuilder) {
		this.email = userBuilder.email;
		this.firstName = userBuilder.firstName;
		this.lastName = userBuilder.lastName;
		this.address = userBuilder.address;
		this.portfolio = new Portfolio();
		this.friends = new ArrayList<>();
		this.hasPremium = false;
	}

	public String getEmail() {
		return email;
	}

	public String getFirstName() {
		return firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public String getAddress() {
		return address;
	}

	@JsonIgnore
	public Portfolio getPortfolio() {
		return portfolio;
	}

	public ArrayList<String> getFriends() {
		return friends;
	}

	public boolean hasPremium() {
		return hasPremium;
	}

	public void upgradeToPremium() {
		this.hasPremium = true;
	}

	public static class UserBuilder {
		private String email;
		private String firstName;
		private String lastName;
		private String address;

		public UserBuilder setEmail(String email) {
			this.email = email;
			return this;
		}

		public UserBuilder setFirstName(String firstName) {
			this.firstName = firstName;
			return this;
		}

		public UserBuilder setLastName(String lastName) {
			this.lastName = lastName;
			return this;
		}

		public UserBuilder setAddress(String address) {
			this.address = address;
			return this;
		}

		public User build() {
			return new User(this);
		}
	}
}
