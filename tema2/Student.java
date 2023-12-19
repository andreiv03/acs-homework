package org.example;

import java.util.LinkedList;
import java.util.Queue;

public class Student {
	private final Queue<Course<?>> preferences = new LinkedList<>();
	private String name;
	private double grade;
	private Course<?> course;

	public Student(String name) {
		this.name = name;
		this.grade = 0.0;
		this.course = null;
	}

	public Queue<Course<?>> getPreferences() {
		return preferences;
	}

	public void addPreference(Course<?> course) {
		preferences.add(course);
	}

	public void removePreference() {
		preferences.remove();
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public double getGrade() {
		return grade;
	}

	public void setGrade(double grade) {
		this.grade = grade;
	}

	public Course<?> getCourse() {
		return course;
	}

	public void setCourse(Course<?> course) {
		this.course = course;
	}
}
