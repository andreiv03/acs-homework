package org.example.production;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.example.IMDB;
import org.example.enumerations.Genre;
import org.example.interfaces.Observer;
import org.example.interfaces.Subject;
import org.example.user.Regular;
import org.example.user.Staff;
import org.example.user.User;

import java.util.ArrayList;

// @formatter:off
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.EXISTING_PROPERTY, property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = Movie.class, name = "Movie"),
		@JsonSubTypes.Type(value = Series.class, name = "Series")
})
// @formatter:on
public abstract class Production implements Comparable<Production>, Subject {
	private final ArrayList<Observer> observers = new ArrayList<>();
	protected String title;
	protected String type;
	protected ArrayList<String> directors;
	protected ArrayList<String> actors;
	protected ArrayList<Genre> genres;
	protected ArrayList<Rating> ratings;
	protected String plot;
	protected double averageRating;
	protected String responsible;

	public Production() {
		this.title = null;
		this.type = null;
		this.directors = null;
		this.actors = null;
		this.genres = null;
		this.ratings = null;
		this.plot = null;
		this.averageRating = 0;
		this.responsible = "ADMIN";
	}

	public Production(String title, String type, ArrayList<String> directors, ArrayList<String> actors,
	                  ArrayList<Genre> genres, ArrayList<Rating> ratings, String plot, double averageRating) {
		this.title = title;
		this.type = type;
		this.directors = directors;
		this.actors = actors;
		this.genres = genres;
		this.ratings = ratings;
		this.plot = plot;
		this.averageRating = averageRating;
		this.responsible = "ADMIN";

		for (Rating rating : this.getRatings()) {
			rating.setProductionTitle(this.title);

			for (User user : IMDB.getInstance().getUsers())
				if (user.getUsername().equals(rating.getUsername()))
					this.registerObserver((Observer) user);
		}
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public ArrayList<String> getDirectors() {
		return directors;
	}

	public void setDirectors(ArrayList<String> directors) {
		this.directors = directors;
	}

	public ArrayList<String> getActors() {
		return actors;
	}

	public void setActors(ArrayList<String> actors) {
		this.actors = actors;
	}

	public ArrayList<Genre> getGenres() {
		return genres;
	}

	public void setGenres(ArrayList<Genre> genres) {
		this.genres = genres;
	}

	public ArrayList<Rating> getRatings() {
		return ratings;
	}

	public void setRatings(ArrayList<Rating> ratings) {
		this.ratings = ratings;

		for (Rating rating : this.getRatings()) {
			rating.setProductionTitle(this.title);

			for (User user : IMDB.getInstance().getUsers())
				if (user.getUsername().equals(rating.getUsername()))
					this.registerObserver((Observer) user);
		}
	}

	public String getPlot() {
		return plot;
	}

	public void setPlot(String plot) {
		this.plot = plot;
	}

	public double getAverageRating() {
		return averageRating;
	}

	public void setAverageRating(double averageRating) {
		this.averageRating = averageRating;
	}

	public String getResponsible() {
		return responsible;
	}

	public void setResponsible(String responsible) {
		this.responsible = responsible;
	}

	public abstract void displayInfo();

	@Override
	public int compareTo(Production production) {
		return this.title.compareTo(production.title);
	}

	@Override
	public void registerObserver(Observer observer) {
		if (!observers.contains(observer))
			observers.add(observer);
	}

	@Override
	public void removeObserver(Observer observer) {
		observers.remove(observer);
	}

	@Override
	public void notifyObservers() {
		for (Observer observer : observers) {
			if (observer instanceof Regular)
				observer.receiveNotification(
						"The production \"" + this.title + "\" you reviewed just received a new review from another " +
								"user!");

			if (observer instanceof Staff)
				observer.receiveNotification(
						"The production \"" + this.title + "\" you are responsible for just received a new review from" +
								" a user!");
		}
	}
}
