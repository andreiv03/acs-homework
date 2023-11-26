package TemaTest.models;

import java.util.ArrayList;
import java.util.Arrays;

public class User {
  private String id;
  private String username;
  private String password;
  private ArrayList<String> followers;
  private ArrayList<String> following;
  private ArrayList<String> posts;
  private ArrayList<String> comments;

  public User(String id, String username, String password, String[] followers, String[] following, String[] posts, String[] comments) {
    this.id = id;
    this.username = username;
    this.password = password;
    this.followers = new ArrayList<>(Arrays.asList(followers));
    this.following = new ArrayList<>(Arrays.asList(following));
    this.posts = new ArrayList<>(Arrays.asList(posts));
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

  public String getPassword() {
    return password;
  }

  public void setPassword(String password) {
    this.password = password;
  }

  public ArrayList<String> getFollowers() {
    return followers;
  }

  public void setFollowers(ArrayList<String> followers) {
    this.followers = followers;
  }

  public ArrayList<String> getFollowing() {
    return following;
  }

  public void setFollowing(ArrayList<String> followings) {
    this.following = followings;
  }

  public ArrayList<String> getPosts() {
    return posts;
  }

  public void setPosts(ArrayList<String> posts) {
    this.posts = posts;
  }

  public ArrayList<String> getComments() {
    return comments;
  }

  public void setComments(ArrayList<String> comments) {
    this.comments = comments;
  }
}
