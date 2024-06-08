package TemaTest.services;

import TemaTest.utils.CSVFileMetadata;

import java.io.*;
import java.util.ArrayList;

public class Service {
  public final CSVFileMetadata usersCSVFileMetadata = new CSVFileMetadata(
      "src/main/resources/users.csv",
      "Id,Username,Password\nFollowers\nFollowing\nPosts\nComments",
      5
  );

  public final CSVFileMetadata postsCSVFileMetadata = new CSVFileMetadata(
      "src/main/resources/posts.csv",
      "Id,Username,Text,Date\nLikes\nComments",
      3
  );

  public final CSVFileMetadata commentsCSVFileMetadata = new CSVFileMetadata(
      "src/main/resources/comments.csv",
      "Id,PostId,Username,Text,Date\nLikes",
      2
  );

  protected ArrayList<String[]> readFromCSV(CSVFileMetadata csvFileMetadata) {
    ArrayList<String[]> records = new ArrayList<>();

    try (BufferedReader bufferedReader = new BufferedReader(new FileReader(csvFileMetadata.getFilePath()))) {
      for (int index = 0; index < csvFileMetadata.getHeaderRowsCount(); ++index)
        bufferedReader.readLine();

      String line;

      while ((line = bufferedReader.readLine()) != null) {
        String[] fields = line.split(",");
        records.add(fields);
      }
    } catch (IOException exception) {
      System.out.println(exception.getMessage());
    }

    return records;
  }

  protected void writeToCSV(CSVFileMetadata csvFileMetadata, ArrayList<String[]> records) {
    try (FileWriter fileWriter = new FileWriter(csvFileMetadata.getFilePath(), true);
         BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
         PrintWriter printWriter = new PrintWriter(bufferedWriter)) {
      for (String[] record : records)
        printWriter.println(String.join(",", record));
    } catch (IOException exception) {
      System.out.println(exception.getMessage());
    }
  }

  protected void deleteRecordFromCSV(CSVFileMetadata csvFileMetadata, String searchValue) {
    ArrayList<String[]> records = readFromCSV(csvFileMetadata);
    cleanupCSV(csvFileMetadata);

    for (int recordIndex = 0;
         recordIndex < records.size();
         recordIndex = recordIndex + csvFileMetadata.getHeaderRowsCount()) {
      String[] record = records.get(recordIndex);

      if (record[0].equals(searchValue))
        for (int index = 0; index < csvFileMetadata.getHeaderRowsCount(); ++index)
          records.remove(recordIndex);
    }

    writeToCSV(csvFileMetadata, records);
  }

  public void cleanupCSV(CSVFileMetadata csvFileMetadata) {
    try (FileWriter fileWriter = new FileWriter(csvFileMetadata.getFilePath(), false);
         BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
         PrintWriter printWriter = new PrintWriter(bufferedWriter)) {
      printWriter.println(csvFileMetadata.getHeader());
    } catch (IOException exception) {
      System.out.println(exception.getMessage());
    }
  }
}
