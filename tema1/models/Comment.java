package TemaTest.models;

import java.util.ArrayList;
import java.util.Arrays;

public class Comment {
  private String id;
  private String postId;
  private String username;
  private String text;
  private String date;
  private ArrayList<String> likes;

  public Comment(String id, String postId, String username, String text, String date, String[] likes) {
    this.id = id;
    this.postId = postId;
    this.username = username;
    this.text = text;
    this.date = date;
    this.likes = new ArrayList<>(Arrays.asList(likes));
  }

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }

  public String getPostId() {
    return postId;
  }

  public void setPostId(String postId) {
    this.postId = postId;
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
}
