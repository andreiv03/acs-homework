package TemaTest.utils;

import java.util.ArrayList;

public class Response {
  private final String status;
  private final StringBuilder message;

  public Response(String status, String message) {
    this.status = status;
    this.message = new StringBuilder("'" + message + "'");
    this.printAsJSON();
  }

  public Response(String status, ArrayList<String> records) {
    this.status = status;
    this.message = new StringBuilder("[");

    for (int recordIndex = 0; recordIndex < records.size(); ++recordIndex) {
      this.message.append("'");
      this.message.append(records.get(recordIndex));
      this.message.append("'");

      if (recordIndex < records.size() - 1)
        this.message.append(",");
    }

    this.message.append("]");
    this.printAsJSON();
  }

  public Response(String status, String[] keys, ArrayList<String[]> records, String... params) {
    this.status = status;
    this.message = new StringBuilder("[");

    for (int recordIndex = 0; recordIndex < records.size(); ++recordIndex) {
      String[] record = records.get(recordIndex);
      this.message.append("{");

      for (int keyIndex = 0; keyIndex < keys.length; ++keyIndex) {
        this.message.append("'");
        this.message.append(keys[keyIndex]);
        this.message.append("':");

        if (record[keyIndex].startsWith("["))
          this.message.append(" ");
        else
          this.message.append("'");

        this.message.append(record[keyIndex]);

        if (record[keyIndex].startsWith("["))
          this.message.append(" ");
        else
          this.message.append("'");

        if (keyIndex < keys.length - 1)
          this.message.append(",");
      }

      this.message.append("}");

      if (recordIndex < records.size() - 1)
        this.message.append(",");
    }

    if (params.length > 0 && params[0].equals("b"))
      this.message.append(" ");

    this.message.append("]");

    if (params.length > 0 && params[0].equals("a"))
      this.message.append(" ");

    this.printAsJSON();
  }

  public void printAsJSON() {
    System.out.print("{");
    System.out.print("'status':'" + status + "',");
    System.out.print("'message': " + message);
    System.out.print("}");
  }
}
