package TemaTest.utils;

public class CSVFileMetadata {
  private final String filePath;
  private final String header;
  private final int headerRowsCount;

  public CSVFileMetadata(String filePath, String header, int headerRowsCount) {
    this.filePath = filePath;
    this.header = header;
    this.headerRowsCount = headerRowsCount;
  }

  public String getFilePath() {
    return filePath;
  }

  public String getHeader() {
    return header;
  }

  public int getHeaderRowsCount() {
    return headerRowsCount;
  }
}
