package org.example.production;

import com.fasterxml.jackson.annotation.JsonTypeName;
import org.example.enumerations.Genre;

import java.util.ArrayList;
import java.util.Map;

@JsonTypeName("Series")
public class Series extends Production {
	private int releaseYear;
	private int numSeasons;
	private Map<String, ArrayList<Episode>> seasons;

	public Series() {
		super();
		this.setType("Series");
		this.releaseYear = 0;
		this.numSeasons = 0;
		this.seasons = null;
	}

	public Series(String title, String type, ArrayList<String> directors, ArrayList<String> actors,
	              ArrayList<Genre> genres, ArrayList<Rating> ratings, String plot, double averageRating, int releaseYear,
	              int numSeasons, Map<String, ArrayList<Episode>> seasons) {
		super(title, type, directors, actors, genres, ratings, plot, averageRating);
		this.releaseYear = releaseYear;
		this.numSeasons = numSeasons;
		this.seasons = seasons;
	}

	public int getReleaseYear() {
		return releaseYear;
	}

	public void setReleaseYear(int releaseYear) {
		this.releaseYear = releaseYear;
	}

	public int getNumSeasons() {
		return numSeasons;
	}

	public void setNumSeasons(int numSeasons) {
		this.numSeasons = numSeasons;
	}

	public Map<String, ArrayList<Episode>> getSeasons() {
		return seasons;
	}

	public void setSeasons(Map<String, ArrayList<Episode>> seasons) {
		this.seasons = seasons;
	}

	@Override
	public void displayInfo() {
		if (this.title != null)
			System.out.println("Title: " + this.title);

		System.out.println("Type: Series");

		if (this.directors != null)
			System.out.println("Directors: " + this.directors);

		if (this.actors != null)
			System.out.println("Actors: " + this.actors);

		if (this.genres != null)
			System.out.println("Genres: " + this.genres);

		if (this.ratings != null)
			System.out.println("Ratings: " + this.ratings);

		if (this.plot != null)
			System.out.println("Plot: " + this.plot);

		if (this.averageRating != 0)
			System.out.println("Average rating: " + this.averageRating);

		if (this.releaseYear != 0)
			System.out.println("Release year: " + this.releaseYear);

		if (this.numSeasons != 0)
			System.out.println("Number of seasons: " + this.numSeasons);

		if (this.seasons != null)
			System.out.println("Seasons: " + this.seasons);

		System.out.println();
	}
}
