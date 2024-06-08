package TemaTest.services;

import TemaTest.models.Comment;
import TemaTest.models.Post;
import TemaTest.models.User;
import TemaTest.utils.Response;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;

public class CommentService extends Service {
  private static int idCounter = 0;
  UserService userService;
  PostService postService;

  public CommentService() {
    this.userService = new UserService();
    this.postService = new PostService();
  }

  public void resetIdCounter() {
    idCounter = 0;
  }

  public Comment getComment(String id) {
    ArrayList<String[]> records = readFromCSV(commentsCSVFileMetadata);

    for (int index = 0; index < records.size(); index = index + commentsCSVFileMetadata.getHeaderRowsCount()) {
      String[] record = records.get(index);

      if (record[0].equals(id)) {
        String postId = record[1];
        String username = record[2];
        String text = record[3];
        String date = record[4];
        String[] likes = records.get(index + 1);
        return new Comment(record[0], postId, username, text, date, likes);
      }
    }

    return null;
  }

  public boolean createComment(String username, String password, String postId, String text) {
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
      new Response("error", "Comment text length exceeded");
      return false;
    }

    if (postId == null) {
      new Response("error", "No post identifier was provided");
      return false;
    }

    Post post = postService.getPost(postId);

    if (post == null) {
      new Response("error", "The post identifier was not valid");
      return false;
    }

    idCounter = idCounter + 1;

    user.setComments(new ArrayList<>() {{
      if (!user.getComments().get(0).equals("#"))
        addAll(user.getComments());

      add(String.valueOf(idCounter));
    }});

    ArrayList<String[]> userData = new ArrayList<>();
    userData.add(new String[]{user.getId(), user.getUsername(), user.getPassword()});
    userData.add(new String[]{String.join(",", user.getFollowers())});
    userData.add(new String[]{String.join(",", user.getFollowing())});
    userData.add(new String[]{String.join(",", user.getPosts())});
    userData.add(new String[]{String.join(",", user.getComments())});

    deleteRecordFromCSV(usersCSVFileMetadata, user.getId());
    writeToCSV(usersCSVFileMetadata, userData);

    post.setComments(new ArrayList<>() {{
      if (!post.getComments().get(0).equals("#"))
        addAll(post.getComments());

      add(String.valueOf(idCounter));
    }});

    ArrayList<String[]> postData = new ArrayList<>();
    postData.add(new String[]{post.getId(), post.getUsername(), post.getText(), post.getDate()});
    postData.add(new String[]{String.join(",", post.getLikes())});
    postData.add(new String[]{String.join(",", post.getComments())});

    deleteRecordFromCSV(postsCSVFileMetadata, postId);
    writeToCSV(postsCSVFileMetadata, postData);

    LocalDate date = LocalDate.now();
    DateTimeFormatter dateFormatter = DateTimeFormatter.ofPattern("dd-MM-yyyy");

    ArrayList<String[]> commentData = new ArrayList<>();
    commentData.add(new String[]{String.valueOf(idCounter), postId, username, text, date.format(dateFormatter)});
    commentData.add(new String[]{"#"});

    writeToCSV(commentsCSVFileMetadata, commentData);
    return true;
  }

  public boolean deleteComment(String username, String password, String commentId) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (commentId == null) {
      new Response("error", "No identifier was provided");
      return false;
    }

    Comment comment = getComment(commentId);

    if (comment == null || !comment.getUsername().equals(username)) {
      new Response("error", "The identifier was not valid");
      return false;
    }

    deleteRecordFromCSV(commentsCSVFileMetadata, commentId);
    return true;
  }

  public boolean likeComment(String username, String password, String commentId, boolean like) {
    User user = userService.getUser(username);

    if (user == null || !user.getPassword().equals(password)) {
      new Response("error", "Login failed");
      return false;
    }

    if (commentId == null) {
      new Response("error", "No comment identifier to " + (like ? "like" : "unlike") + " was provided");
      return false;
    }

    Comment comment = getComment(commentId);

    if ((comment == null || comment.getLikes().contains(username)) && like) {
      new Response("error", "The comment identifier to like was not valid");
      return false;
    }

    if ((comment == null || !comment.getLikes().contains(username)) && !like) {
      new Response("error", "The comment identifier to unlike was not valid");
      return false;
    }

    comment.setLikes(new ArrayList<>() {{
      if (!comment.getLikes().get(0).equals("#"))
        addAll(comment.getLikes());

      if (like) add(username);
      else remove(username);
    }});

    if (comment.getLikes().isEmpty())
      comment.setLikes(new ArrayList<>() {{
        add("#");
      }});

    ArrayList<String[]> commentData = new ArrayList<>();
    commentData.add(new String[]{comment.getId(), comment.getPostId(), comment.getUsername(), comment.getText(), comment.getDate()});
    commentData.add(new String[]{String.join(",", comment.getLikes())});

    deleteRecordFromCSV(commentsCSVFileMetadata, commentId);
    writeToCSV(commentsCSVFileMetadata, commentData);

    return true;
  }
}
