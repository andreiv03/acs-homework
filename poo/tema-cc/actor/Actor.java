package org.example.actor;

import java.util.ArrayList;

public class Actor implements Comparable<Actor> {
	private String name;
	private ArrayList<Performance> performances;
	private String biography;
	private String responsible;

	public Actor() {
		this.name = null;
		this.performances = new ArrayList<>();
		this.biography = null;
		this.responsible = "ADMIN";
	}

	public Actor(String name, ArrayList<Performance> performances, String biography) {
		this.name = name;
		this.performances = performances;
		this.biography = biography;
		this.responsible = "ADMIN";
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public ArrayList<Performance> getPerformances() {
		return performances;
	}

	public void setPerformances(ArrayList<Performance> performances) {
		this.performances = performances;
	}

	public String getBiography() {
		return biography;
	}

	public void setBiography(String biography) {
		this.biography = biography;
	}

	public String getResponsible() {
		return responsible;
	}

	public void setResponsible(String responsible) {
		this.responsible = responsible;
	}

	public void displayInfo() {
		if (this.name != null)
			System.out.println("Name: " + this.name);

		if (this.performances != null)
			System.out.println("Performances: " + this.performances);

		if (this.biography != null)
			System.out.println("Biography: " + this.biography);

		System.out.println();
	}

	@Override
	public int compareTo(Actor actor) {
		return this.name.compareTo(actor.name);
	}

	public static class Performance {
		private String title;
		private String type;

		public Performance() {
			this.title = null;
			this.type = null;
		}

		public Performance(String title, String type) {
			this.title = title;
			this.type = type;
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

		@Override
		public String toString() {
			return "{ title: \"" + title + ", type: \"" + type + "\" }";
		}
	}
}
