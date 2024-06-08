package org.example.production;

public class Episode {
	private String episodeName;
	private String duration;

	public Episode() {
		this.episodeName = null;
		this.duration = null;
	}

	public Episode(String episodeName, String duration) {
		this.episodeName = episodeName;
		this.duration = duration;
	}

	public String getEpisodeName() {
		return episodeName;
	}

	public void setEpisodeName(String episodeName) {
		this.episodeName = episodeName;
	}

	public String getDuration() {
		return duration;
	}

	public void setDuration(String duration) {
		this.duration = duration;
	}

	@Override
	public String toString() {
		return "{ episodeName: \"" + episodeName + "\", duration: \"" + duration + "\" }";
	}
}
