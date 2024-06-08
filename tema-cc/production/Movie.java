package org.example.production;

import com.fasterxml.jackson.annotation.JsonTypeName;
import org.example.enumerations.Genre;

import java.util.ArrayList;

@JsonTypeName("Movie")
public class Movie extends Production {
	private String duration;
	private int releaseYear;

	public Movie() {
		super();
		this.setType("Movie");
		this.duration = null;
		this.releaseYear = 0;
	}

	public Movie(String title, String type, ArrayList<String> directors, ArrayList<String> actors,
	             ArrayList<Genre> genres, ArrayList<Rating> ratings, String plot, double averageRating, String duration,
	             int releaseYear) {
		super(title, type, directors, actors, genres, ratings, plot, averageRating);
		this.duration = duration;
		this.releaseYear = releaseYear;
	}

	public String getDuration() {
		return duration;
	}

	public void setDuration(String duration) {
		this.duration = duration;
	}

	public int getReleaseYear() {
		return releaseYear;
	}

	public void setReleaseYear(int releaseYear) {
		this.releaseYear = releaseYear;
	}

	@Override
	public void displayInfo() {
		if (this.title != null)
			System.out.println("Title: " + this.title);

		System.out.println("Type: Movie");

		if (this.directors != null)
			System.out.println("Directors: " + this.directors);

		if (this.actors != null)
			System.out.println("Actors: " + this.actors);

		if (this.genres != null)
			System.out.println("Genres: " + this.genres);

		if (this.ratings != null) {
			this.ratings.sort(null);
			System.out.println("Ratings: " + this.ratings);
		}

		if (this.plot != null)
			System.out.println("Plot: " + this.plot);

		if (this.averageRating != 0)
			System.out.println("Average rating: " + this.averageRating);

		if (this.duration != null)
			System.out.println("Duration: " + this.duration);

		if (this.releaseYear != 0)
			System.out.println("Release year: " + this.releaseYear);

		System.out.println();
	}
}
