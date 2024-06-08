package org.example.request;

import org.example.enumerations.RequestStatus;
import org.example.enumerations.RequestType;
import org.example.interfaces.Observer;
import org.example.interfaces.Subject;
import org.example.user.Regular;
import org.example.user.Staff;
import org.example.utils.Utils;

import java.time.LocalDateTime;
import java.util.ArrayList;

public class Request implements Subject {
	private final ArrayList<Observer> observers = new ArrayList<>();
	private RequestType type;
	private LocalDateTime createdDate;
	private String username;
	private String actorName;
	private String movieTitle;
	private String to;
	private String description;
	private String id;
	private RequestStatus status;

	public Request() {
		this.type = null;
		this.createdDate = null;
		this.username = null;
		this.actorName = null;
		this.movieTitle = null;
		this.to = null;
		this.description = null;
		this.id = Utils.generateId();
		this.status = RequestStatus.PENDING;
	}

	public Request(RequestType type, LocalDateTime createdDate, String username, String actorName, String movieTitle,
	               String to, String description) {
		this.type = type;
		this.createdDate = createdDate;
		this.username = username;
		this.actorName = actorName;
		this.movieTitle = movieTitle;
		this.to = to;
		this.description = description;
		this.id = Utils.generateId();
		this.status = RequestStatus.PENDING;
	}

	public RequestType getType() {
		return type;
	}

	public void setType(RequestType type) {
		this.type = type;
	}

	public LocalDateTime getCreatedDate() {
		return createdDate;
	}

	public void setCreatedDate(LocalDateTime createdDate) {
		this.createdDate = createdDate;
	}

	public String getUsername() {
		return username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public String getActorName() {
		return actorName;
	}

	public void setActorName(String actorName) {
		this.actorName = actorName;
	}

	public String getMovieTitle() {
		return movieTitle;
	}

	public void setMovieTitle(String movieTitle) {
		this.movieTitle = movieTitle;
	}

	public String getTo() {
		return to;
	}

	public void setTo(String to) {
		this.to = to;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public RequestStatus getStatus() {
		return status;
	}

	public void setStatus(RequestStatus status) {
		this.status = status;
	}

	@Override
	public String toString() {
		return "{ id: \"" + id + "\", type: \"" + type + "\", createdDate: \"" + createdDate + (actorName != null ?
				"\", " + "actorName: \"" + actorName : "") + (movieTitle != null ? "\", movieTitle: \"" + movieTitle : "") +
				"\", " + "description: \"" + description + "\" }";
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
				observer.receiveNotification("Your request (id: " + this.id + ") has been " + (this.getStatus().equals(
						RequestStatus.APPROVED) ? "approved" : "rejected") + "!");

			if (observer instanceof Staff)
				observer.receiveNotification("A new request (id: " + this.id + ") has been " + (this.getStatus().equals(
						RequestStatus.APPROVED) ? "approved" : "rejected") + "!");
		}
	}
}
