package TemaTest.services;

import TemaTest.models.Comment;
import TemaTest.models.Post;
import TemaTest.models.User;
import TemaTest.utils.Response;

import java.util.ArrayList;
import java.util.Collections;

public class UserService extends Service {
  private static int idCounter = 0;

  public void resetIdCounter() {
    idCounter = 0;
  }

  public User getUser(String username) {
    ArrayList<String[]> records = readFromCSV(usersCSVFileMetadata);

    for (int index = 0; index < records.size(); index = index + usersCSVFileMetadata.getHeaderRowsCount()) {
      String[] record = records.get(index);

      if (record[1].equals(username)) {
        String id = record[0];
        String password = record[2];
        String[] followers = records.get(index + 1);
        String[] following = records.get(index + 2);
        String[] posts = records.get(index + 3);
        String[] comments = records.get(index + 4);
        return new User(id, record[1], password, followers, following, posts, comments);
      }
    }

    return null;
  }

  public boolean createUser(String username, String password) {
    if (getUser(username) != null) {
      new Response("error", "User already exists");
      return false;
    }

    idCounter = idCounter + 1;

    ArrayList<String[]> userData = new ArrayList<>();
    userData.add(new String[]{String.valueOf(idCounter), username, password});
    userData.add(new String[]{"#"});
    userData.add(new String[]{"#"});
    userData.add(new String[]{"#"});
    userData.add(new String[]{"#"});

    writeToCSV(usersCSVFileMetadata, userData);
    return true;
  }

  public boolean followUser(String username, String password, String followingUsername, boolean follow) {
    User user = getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (followingUsername == null) {
      new Response("error", "No username to " + (follow ? "follow" : "unfollow") + " was provided");
      return false;
    }

    User following = getUser(followingUsername);

    if ((following == null || user.getFollowing().contains(followingUsername)) && follow) {
      new Response("error", "The username to follow was not valid");
      return false;
    }

    if ((following == null || !user.getFollowing().contains(followingUsername)) && !follow) {
      new Response("error", "The username to unfollow was not valid");
      return false;
    }

    user.setFollowing(new ArrayList<>() {{
      if (!user.getFollowing().get(0).equals("#"))
        addAll(user.getFollowing());

      if (follow) add(followingUsername);
      else remove(followingUsername);
    }});

    if (user.getFollowing().isEmpty())
      user.setFollowing(new ArrayList<>() {{
        add("#");
      }});

    ArrayList<String[]> userData = new ArrayList<>();
    userData.add(new String[]{user.getId(), username, password});
    userData.add(new String[]{String.join(",", user.getFollowers())});
    userData.add(new String[]{String.join(",", user.getFollowing())});
    userData.add(new String[]{String.join(",", user.getPosts())});
    userData.add(new String[]{String.join(",", user.getComments())});

    following.setFollowers(new ArrayList<>() {{
      if (!following.getFollowers().get(0).equals("#"))
        addAll(following.getFollowers());

      if (follow) add(username);
      else remove(username);
    }});

    if (following.getFollowers().isEmpty())
      following.setFollowers(new ArrayList<>() {{
        add("#");
      }});

    ArrayList<String[]> followingData = new ArrayList<>();
    followingData.add(new String[]{following.getId(), followingUsername, following.getPassword()});
    followingData.add(new String[]{String.join(",", following.getFollowers())});
    followingData.add(new String[]{String.join(",", following.getFollowing())});
    followingData.add(new String[]{String.join(",", following.getPosts())});
    followingData.add(new String[]{String.join(",", following.getComments())});

    deleteRecordFromCSV(usersCSVFileMetadata, user.getId());
    deleteRecordFromCSV(usersCSVFileMetadata, following.getId());

    writeToCSV(usersCSVFileMetadata, userData);
    writeToCSV(usersCSVFileMetadata, followingData);

    return true;
  }

  public ArrayList<String> getFollowers(String username, String password, String targetUsername) {
    User user = getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    if (targetUsername == null) {
      new Response("error", "No username to list followers was provided");
      return null;
    }

    User targetUser = getUser(targetUsername);

    if (targetUser == null) {
      new Response("error", "The username to list followers was not valid");
      return null;
    }

    ArrayList<String> records = new ArrayList<>(0);

    for (String followerUsername : targetUser.getFollowers())
      if (!followerUsername.equals("#"))
        records.add(followerUsername);

    return records;
  }

  public ArrayList<String> getFollowing(String username, String password) {
    User user = getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String> records = new ArrayList<>(0);

    for (String followingUsername : user.getFollowing())
      if (!followingUsername.equals("#"))
        records.add(followingUsername);

    return records;
  }

  public ArrayList<String[]> getMostFollowedUsers(String username, String password) {
    User user = getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String[]> users = readFromCSV(usersCSVFileMetadata);

    for (int index = 0; index < users.size(); index = index + usersCSVFileMetadata.getHeaderRowsCount()) {
      String[] record = users.get(index);
      String[] followers = users.get(index + 1);

      String numberOfFollowers = !followers[0].equals("#") ? String.valueOf(followers.length) : "0";
      records.add(new String[]{record[1], numberOfFollowers});
    }

    records.sort((a, b) -> Integer.parseInt(b[1]) - Integer.parseInt(a[1]));
    records = new ArrayList<>(records.subList(0, Math.min(records.size(), 5)));

    return records;
  }

  public ArrayList<String[]> getMostLikedUsers(String username, String password) {
    User user = getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String[]> users = readFromCSV(usersCSVFileMetadata);

    for (int indexOne = 0; indexOne < users.size(); indexOne = indexOne + 5)
      for (int indexTwo = indexOne + 5; indexTwo < users.size(); indexTwo = indexTwo + 5)
        if (Integer.parseInt(users.get(indexOne)[0]) > Integer.parseInt(users.get(indexTwo)[0]))
          for (int index = 0; index < 5; ++index)
            Collections.swap(users, indexOne + index, indexTwo + index);

    for (int index = 0; index < users.size(); index = index + usersCSVFileMetadata.getHeaderRowsCount()) {
      String[] record = users.get(index);
      String[] posts = users.get(index + 3);
      String[] comments = users.get(index + 4);

      int numberOfLikes = 0;

      for (String postId : posts) {
        Post post = new PostService().getPost(postId);

        if (post == null)
          continue;

        ArrayList<String> likes = post.getLikes();
        numberOfLikes = numberOfLikes + (!likes.get(0).equals("#") ? likes.size() : 0);
      }

      for (String commentId : comments) {
        Comment comment = new CommentService().getComment(commentId);

        if (comment == null)
          continue;

        ArrayList<String> likes = comment.getLikes();
        numberOfLikes = numberOfLikes + (!likes.get(0).equals("#") ? likes.size() : 0);
      }

      records.add(new String[]{record[1], String.valueOf(numberOfLikes)});
    }

    records.sort((a, b) -> Integer.parseInt(b[1]) - Integer.parseInt(a[1]));
    records = new ArrayList<>(records.subList(0, Math.min(records.size(), 5)));

    return records;
  }
}
