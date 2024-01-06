package org.example.production;

import org.example.IMDB;
import org.example.user.User;

public class Rating implements Comparable<Rating> {
	private String username;
	private int rating;
	private String comment;
	private String productionTitle;

	public Rating() {
		this.username = null;
		this.rating = 0;
		this.comment = null;
		this.productionTitle = null;
	}

	public Rating(String username, int rating, String comment) {
		this.username = username;
		this.rating = rating;
		this.comment = comment;
		this.productionTitle = null;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public int getRating() {
		return rating;
	}

	public void setRating(int rating) {
		this.rating = rating;
	}

	public String getComment() {
		return comment;
	}

	public void setComment(String comment) {
		this.comment = comment;
	}

	public String getProductionTitle() {
		return productionTitle;
	}

	public void setProductionTitle(String productionTitle) {
		this.productionTitle = productionTitle;
	}

	@Override
	public String toString() {
		return "{ username: \"" + username + "\", productionTitle: \"" + productionTitle + "\", rating: \"" + rating +
				"\", comment: \"" + comment + "\" }";
	}

	@Override
	public int compareTo(Rating rating) {
		User userOne = IMDB.getInstance().getUsers().stream().filter(
				user -> user.getUsername().equals(this.username)).findFirst().orElse(null);
		User userTwo = IMDB.getInstance().getUsers().stream().filter(
				user -> user.getUsername().equals(rating.getUsername())).findFirst().orElse(null);

		if (userOne == null || userTwo == null)
			return 0;

		return Integer.parseInt(userTwo.getExperience()) - Integer.parseInt(userOne.getExperience());
	}
}
