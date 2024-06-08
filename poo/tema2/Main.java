package org.example;

import org.example.exceptions.StudentAlreadyExists;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

public class Main {
	public static void main(String[] args) {
		final String path = "src/main/resources/";
		final String inputFilePath = path + args[0] + "/" + args[0] + ".in";
		final String outputFilePath = path + args[0] + "/" + args[0] + ".out";

		ArrayList<String> commands = readFromFile(inputFilePath);

		if (commands == null)
			return;

		Secretariat secretariat = new Secretariat();

		for (String command : commands) {
			String[] commandParts = command.split("-");

			for (int index = 0; index < commandParts.length; ++index)
				commandParts[index] = commandParts[index].trim();

			String commandName = commandParts[0];

			try {
				if (commandName.equals("adauga_student")) {
					String studiesProgram = commandParts[1];
					String studentName = commandParts[2];

					secretariat.addStudent(studiesProgram, studentName);
				}

				if (commandName.equals("adauga_curs")) {
					String studiesProgram = commandParts[1];
					String courseName = commandParts[2];
					int capacity = Integer.parseInt(commandParts[3]);

					secretariat.addCourse(studiesProgram, courseName, capacity);
				}

				if (commandName.equals("citeste_mediile"))
					secretariat.readGrades(path + args[0] + "/");

				if (commandName.equals("posteaza_mediile"))
					secretariat.printGrades(outputFilePath);

				if (commandName.equals("contestatie")) {
					String studentName = commandParts[1];
					String grade = commandParts[2];

					secretariat.contestGrade(studentName, grade);
				}

				if (commandName.equals("adauga_preferinte")) {
					String studentName = commandParts[1];
					ArrayList<String> courseNames = new ArrayList<>(Arrays.asList(commandParts).subList(2, commandParts.length));

					secretariat.addPreferences(studentName, courseNames);
				}

				if (commandName.equals("repartizeaza"))
					secretariat.distributeStudents();

				if (commandName.equals("posteaza_curs")) {
					String courseName = commandParts[1];
					secretariat.printCourse(courseName, outputFilePath);
				}

				if (commandName.equals("posteaza_student")) {
					String studentName = commandParts[1];
					secretariat.printStudent(studentName, outputFilePath);
				}
			} catch (StudentAlreadyExists exception) {
				writeToFile(outputFilePath, "***");
				writeToFile(outputFilePath, exception.getMessage());
			}
		}
	}

	public static ArrayList<String> readFromFile(String path) {
		try (BufferedReader bufferedReader = new BufferedReader(new FileReader(path))) {
			ArrayList<String> lines = new ArrayList<>();

			for (String line; (line = bufferedReader.readLine()) != null; )
				lines.add(line);

			return lines;
		} catch (IOException exception) {
			System.out.println(exception.getMessage());
			return null;
		}
	}

	public static void writeToFile(String path, String line) {
		try (FileWriter fileWriter = new FileWriter(path, true)) {
			try (BufferedWriter bufferedWriter = new BufferedWriter(fileWriter)) {
				try (PrintWriter printWriter = new PrintWriter(bufferedWriter)) {
					printWriter.println(line);
				}
			}
		} catch (IOException exception) {
			System.out.println(exception.getMessage());
		}
	}
}
