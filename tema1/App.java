package TemaTest;

import TemaTest.commands.*;
import TemaTest.interfaces.Command;
import TemaTest.utils.CommandsMap;

import java.util.ArrayList;

public class App {
  public static void main(String[] args) {
    if (args == null) {
      System.out.println("Hello world!");
      return;
    }

    CommandsMap commands = new CommandsMap();
    commands.add("-create-user", new CreateUser());
    commands.add("-create-post", new CreatePost());
    commands.add("-delete-post-by-id", new DeletePostById());
    commands.add("-follow-user-by-username", new FollowUserByUsername());
    commands.add("-unfollow-user-by-username", new UnfollowUserByUsername());
    commands.add("-like-post", new LikePost());
    commands.add("-unlike-post", new UnlikePost());
    commands.add("-like-comment", new LikeComment());
    commands.add("-unlike-comment", new UnlikeComment());
    commands.add("-get-followings-posts", new GetFollowingsPosts());
    commands.add("-get-user-posts", new GetUserPosts());
    commands.add("-get-post-details", new GetPostDetails());
    commands.add("-comment-post", new CommentPost());
    commands.add("-delete-comment-by-id", new DeleteCommentById());
    commands.add("-get-following", new GetFollowing());
    commands.add("-get-followers", new GetFollowers());
    commands.add("-get-most-liked-posts", new GetMostLikedPosts());
    commands.add("-get-most-commented-posts", new GetMostCommentedPosts());
    commands.add("-get-most-followed-users", new GetMostFollowedUsers());
    commands.add("-get-most-liked-users", new GetMostLikedUsers());
    commands.add("-cleanup-all", new CleanupAll());

    Command command = commands.get(args[0]);

    ArrayList<String> options = new ArrayList<>(0);
    ArrayList<String> values = new ArrayList<>(0);

    for (int index = 0; index < args.length; ++index) {
      if (index == 0)
        continue;

      String[] parts = args[index].split(" ", 2);
      options.add(parts[0]);
      values.add(parts[1].replace("'", ""));
    }

    if (command != null)
      command.execute(options, values);
  }
}
