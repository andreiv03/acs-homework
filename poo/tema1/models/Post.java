package TemaTest.models;

import java.util.ArrayList;
import java.util.Arrays;

public class Post {
  private String id;
  private String username;
  private String text;
  private String date;
  private ArrayList<String> likes;
  private ArrayList<String> comments;

  public Post(String id, String username, String text, String date, String[] likes, String[] comments) {
    this.id = id;
    this.username = username;
    this.text = text;
    this.date = date;
    this.likes = new ArrayList<>(Arrays.asList(likes));
    this.comments = new ArrayList<>(Arrays.asList(comments));
  }

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }

  public String getUsername() {
    return username;
  }

  public void setUsername(String username) {
    this.username = username;
  }

  public String getText() {
    return text;
  }

  public void setText(String text) {
    this.text = text;
  }

  public String getDate() {
    return date;
  }

  public void setDate(String date) {
    this.date = date;
  }

  public ArrayList<String> getLikes() {
    return likes;
  }

  public void setLikes(ArrayList<String> likes) {
    this.likes = likes;
  }

  public ArrayList<String> getComments() {
    return comments;
  }

  public void setComments(ArrayList<String> comments) {
    this.comments = comments;
  }
}
