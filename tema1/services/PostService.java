package TemaTest.services;

import TemaTest.models.Comment;
import TemaTest.models.Post;
import TemaTest.models.User;
import TemaTest.utils.Response;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;

public class PostService extends Service {
  private static int idCounter = 0;
  UserService userService;

  public PostService() {
    this.userService = new UserService();
  }

  public void resetIdCounter() {
    idCounter = 0;
  }

  public Post getPost(String id) {
    ArrayList<String[]> records = readFromCSV(postsCSVFileMetadata);

    for (int index = 0; index < records.size(); index = index + postsCSVFileMetadata.getHeaderRowsCount()) {
      String[] record = records.get(index);

      if (record[0].equals(id)) {
        String username = record[1];
        String text = record[2];
        String date = record[3];
        String[] likes = records.get(index + 1);
        String[] comments = records.get(index + 2);
        return new Post(record[0], username, text, date, likes, comments);
      }
    }

    return null;
  }

  public boolean createPost(String username, String password, String text) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (text == null) {
      new Response("error", "No text provided");
      return false;
    }

    if (text.length() > 300) {
      new Response("error", "Post text length exceeded");
      return false;
    }

    idCounter = idCounter + 1;

    LocalDate date = LocalDate.now();
    DateTimeFormatter dateFormatter = DateTimeFormatter.ofPattern("dd-MM-yyyy");

    ArrayList<String[]> post = new ArrayList<>();
    post.add(new String[]{String.valueOf(idCounter), username, text, date.format(dateFormatter)});
    post.add(new String[]{"#"});
    post.add(new String[]{"#"});

    writeToCSV(postsCSVFileMetadata, post);

    user.setPosts(new ArrayList<>() {{
      if (!user.getPosts().get(0).equals("#"))
        addAll(user.getPosts());

      add(String.valueOf(idCounter));
    }});

    ArrayList<String[]> userData = new ArrayList<>();
    userData.add(new String[]{user.getId(), username, password});
    userData.add(new String[]{String.join(",", user.getFollowers())});
    userData.add(new String[]{String.join(",", user.getFollowing())});
    userData.add(new String[]{String.join(",", user.getPosts())});
    userData.add(new String[]{String.join(",", user.getComments())});

    deleteRecordFromCSV(usersCSVFileMetadata, user.getId());
    writeToCSV(usersCSVFileMetadata, userData);

    return true;
  }

  public boolean deletePost(String username, String password, String postId) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (postId == null) {
      new Response("error", "No identifier was provided");
      return false;
    }

    Post post = getPost(postId);

    if (post == null) {
      new Response("error", "The identifier was not valid");
      return false;
    }

    deleteRecordFromCSV(postsCSVFileMetadata, postId);
    return true;
  }

  public boolean likePost(String username, String password, String postId, boolean like) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (postId == null) {
      new Response("error", "No post identifier to " + (like ? "like" : "unlike") + " was provided");
      return false;
    }

    Post post = getPost(postId);

    if ((post == null || post.getLikes().contains(username)) && like) {
      new Response("error", "The post identifier to like was not valid");
      return false;
    }

    if ((post == null || !post.getLikes().contains(username)) && !like) {
      new Response("error", "The post identifier to unlike was not valid");
      return false;
    }

    post.setLikes(new ArrayList<>() {{
      if (!post.getLikes().get(0).equals("#"))
        addAll(post.getLikes());

      if (like) add(username);
      else remove(username);
    }});

    if (post.getLikes().isEmpty())
      post.setLikes(new ArrayList<>() {{
        add("#");
      }});

    ArrayList<String[]> postData = new ArrayList<>();
    postData.add(new String[]{post.getId(), post.getUsername(), post.getText(), post.getDate()});
    postData.add(new String[]{String.join(",", post.getLikes())});
    postData.add(new String[]{String.join(",", post.getComments())});

    deleteRecordFromCSV(postsCSVFileMetadata, postId);
    writeToCSV(postsCSVFileMetadata, postData);

    return true;
  }

  public ArrayList<String[]> getPostDetails(String username, String password, String postId) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    if (postId == null) {
      new Response("error", "No post identifier was provided");
      return null;
    }

    Post post = getPost(postId);

    if (post == null) {
      new Response("error", "The post identifier was not valid");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String> record = new ArrayList<>(0);
    record.add(post.getText());
    record.add(post.getDate());
    record.add(post.getUsername());
    record.add(!post.getLikes().get(0).equals("#") ? String.valueOf(post.getLikes().size()) : "0");

    ArrayList<String> comments = post.getComments();
    StringBuilder commentsString = new StringBuilder("[");

    for (int commentIndex = 0; commentIndex < comments.size(); ++commentIndex) {
      Comment comment = new CommentService().getComment(comments.get(commentIndex));

      commentsString.append("{");
      commentsString.append("'comment_id':'");
      commentsString.append(comment.getId());
      commentsString.append("','comment_text':'");
      commentsString.append(comment.getText());
      commentsString.append("','comment_date':'");
      commentsString.append(comment.getDate());
      commentsString.append("','username':'");
      commentsString.append(comment.getUsername());
      commentsString.append("','number_of_likes':'");
      commentsString.append(!comment.getLikes().get(0).equals("#") ? String.valueOf(comment.getLikes().size()) : "0");
      commentsString.append("'}");

      if (commentIndex < comments.size() - 1)
        commentsString.append(",");
    }

    commentsString.append("]");
    record.add(commentsString.toString());
    records.add(record.toArray(new String[0]));

    return records;
  }

  public ArrayList<String[]> getUserPosts(String username, String password, String followingUsername) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    if (followingUsername == null) {
      new Response("error", "No username to list posts was provided");
      return null;
    }

    User followingUser = userService.getUser(followingUsername);

    if (followingUser == null || !user.getFollowing().contains(followingUsername)) {
      new Response("error", "The username to list posts was not valid");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String> posts = followingUser.getPosts();

    for (int postIdIndex = posts.size() - 1; postIdIndex >= 0; --postIdIndex) {
      Post post = getPost(posts.get(postIdIndex));
      records.add(new String[]{post.getId(), post.getText(), post.getDate()});
    }

    return records;
  }

  public ArrayList<String[]> getFollowingsPosts(String username, String password) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String> following = user.getFollowing();

    for (int followingUsernameIndex = following.size() - 1; followingUsernameIndex >= 0; --followingUsernameIndex) {
      User followingUser = userService.getUser(following.get(followingUsernameIndex));

      for (int postIdIndex = followingUser.getPosts().size() - 1; postIdIndex >= 0; --postIdIndex) {
        Post post = getPost(followingUser.getPosts().get(postIdIndex));
        records.add(new String[]{post.getId(), post.getText(), post.getDate(), post.getUsername()});
      }
    }

    return records;
  }

  public ArrayList<String[]> getMostLikedPosts(String username, String password) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String[]> posts = readFromCSV(postsCSVFileMetadata);

    for (int index = 0; index < posts.size(); index = index + postsCSVFileMetadata.getHeaderRowsCount()) {
      String[] post = posts.get(index);
      String[] likes = posts.get(index + 1);

      String postId = post[0];
      String postText = post[2];
      String postDate = post[3];
      String postUsername = post[1];
      String postNumberOfLikes = !likes[0].equals("#") ? String.valueOf(likes.length) : "0";

      records.add(new String[]{postId, postText, postDate, postUsername, postNumberOfLikes});
    }

    records.sort((a, b) -> Integer.parseInt(b[4]) - Integer.parseInt(a[4]));
    records = new ArrayList<>(records.subList(0, Math.min(records.size(), 5)));

    return records;
  }

  public ArrayList<String[]> getMostCommentedPosts(String username, String password) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return null;
    }

    ArrayList<String[]> records = new ArrayList<>(0);
    ArrayList<String[]> posts = readFromCSV(postsCSVFileMetadata);

    for (int index = 0; index < posts.size(); index = index + postsCSVFileMetadata.getHeaderRowsCount()) {
      String[] post = posts.get(index);
      String[] comments = posts.get(index + 2);

      String postId = post[0];
      String postText = post[2];
      String postDate = post[3];
      String postUsername = post[1];
      String postNumberOfComments = !comments[0].equals("#") ? String.valueOf(comments.length) : "0";

      records.add(new String[]{postId, postText, postDate, postUsername, postNumberOfComments});
    }

    records.sort((a, b) -> Integer.parseInt(b[4]) - Integer.parseInt(a[4]));
    records = new ArrayList<>(records.subList(0, Math.min(records.size(), 5)));

    return records;
  }
}
