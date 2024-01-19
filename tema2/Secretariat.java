package org.example;

import org.example.exceptions.StudentAlreadyExists;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Set;
import java.util.TreeSet;

import static org.example.Main.readFromFile;
import static org.example.Main.writeToFile;

public class Secretariat {
	private final Comparator<Course<?>> coursesComparator = Comparator.comparing(Course::getName);
	private final Set<Course<?>> courses = new TreeSet<>(coursesComparator);
	private final ArrayList<Student> students = new ArrayList<>();

	public void addStudent(String studiesProgram, String studentName) throws StudentAlreadyExists {
		for (Student student : students)
			if (student.getName().equals(studentName))
				throw new StudentAlreadyExists("Student duplicat: " + studentName);

		if (studiesProgram.equals("licenta"))
			students.add(new Undergraduate(studentName));

		if (studiesProgram.equals("master"))
			students.add(new Graduate(studentName));
	}

	public void addCourse(String studiesProgram, String courseName, int capacity) {
		if (studiesProgram.equals("licenta"))
			courses.add(new Course<Undergraduate>(courseName, capacity));

		if (studiesProgram.equals("master"))
			courses.add(new Course<Graduate>(courseName, capacity));
	}

	public void readGrades(String path) {
		int filesNumber = 1;

		while (true) {
			String filePath = path + "note_" + filesNumber + ".txt";

			if (!Files.exists(Path.of(filePath)))
				break;

			ArrayList<String> lines = readFromFile(filePath);

			if (lines == null)
				return;

			for (String line : lines) {
				String[] lineParts = line.split("-");

				for (int index = 0; index < lineParts.length; ++index)
					lineParts[index] = lineParts[index].trim();

				String studentName = lineParts[0];
				double grade = Double.parseDouble(lineParts[1]);

				for (Student student : students)
					if (student.getName().equals(studentName))
						student.setGrade(grade);
			}

			filesNumber = filesNumber + 1;
		}
	}

	public void sortStudentsByGrade() {
		students.sort((a, b) -> {
			if (a.getGrade() < b.getGrade())
				return 1;

			if (a.getGrade() > b.getGrade())
				return -1;

			return a.getName().compareTo(b.getName());
		});
	}

	public void printGrades(String path) {
		writeToFile(path, "***");
		sortStudentsByGrade();

		for (Student student : students) {
			String line = student.getName() + " - " + student.getGrade();
			writeToFile(path, line);
		}
	}

	public void contestGrade(String studentName, String grade) {
		for (Student student : students)
			if (student.getName().equals(studentName))
				student.setGrade(Double.parseDouble(grade));
	}

	public void addPreferences(String studentName, ArrayList<String> courseNames) {
		for (Student student : students) {
			if (!student.getName().equals(studentName))
				continue;

			for (String courseName : courseNames) {
				for (Course<?> course : courses) {
					if (!course.getName().equals(courseName))
						continue;

					student.addPreference(course);
				}
			}
		}
	}

	public void distributeStudents() {
		for (Student student : students) {
			while (!student.getPreferences().isEmpty()) {
				Course<?> course = student.getPreferences().peek();

				if (course == null)
					break;

				if (course.getCapacity() > course.getStudents().size()) {
					student.setCourse(course);
					course.addStudent(student);
					break;
				}

				if (course.getCapacity() <= course.getStudents().size()) {
					for (Student studentInCourse : course.getStudents()) {
						if (studentInCourse.getGrade() == student.getGrade()) {
							student.setCourse(course);
							course.addStudent(student);
							break;
						}
					}
				}

				student.removePreference();
			}

			if (student.getCourse() == null) {
				for (Course<?> course : courses) {
					if (course.getCapacity() > course.getStudents().size()) {
						student.setCourse(course);
						course.addStudent(student);
						break;
					}
				}
			}
		}
	}

	public void printCourse(String courseName, String path) {
		writeToFile(path, "***");

		for (Course<?> course : courses) {
			if (!course.getName().equals(courseName))
				continue;

			String line = courseName + " (" + course.getCapacity() + ")";
			writeToFile(path, line);

			for (Student student : course.getStudents()) {
				String studentName = student.getName();
				double studentGrade = student.getGrade();

				line = studentName + " - " + studentGrade;
				writeToFile(path, line);
			}
		}
	}

	public void printStudent(String studentName, String path) {
		writeToFile(path, "***");

		for (Student student : students) {
			if (!student.getName().equals(studentName))
				continue;

			String studiesProgram = student instanceof Undergraduate ? "Licenta" : "Master";
			double studentGrade = student.getGrade();
			String courseName = student.getCourse().getName();

			String line = "Student " + studiesProgram + ": " + studentName + " - " + studentGrade + " - " + courseName;
			writeToFile(path, line);
		}
	}
}
