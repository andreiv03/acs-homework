package org.example;

import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

public class Course<T extends Student> {
	private final Comparator<Student> studentsComparator = Comparator.comparing(Student::getName);
	private final Set<T> students = new TreeSet<>(studentsComparator);
	private String name;
	private int capacity;

	public Course(String name, int capacity) {
		this.name = name;
		this.capacity = capacity;
	}

	public Set<T> getStudents() {
		return students;
	}

	public void addStudent(Student student) {
		students.add((T) student);
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getCapacity() {
		return capacity;
	}

	public void setCapacity(int capacity) {
		this.capacity = capacity;
	}
}
