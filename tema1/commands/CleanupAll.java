package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.CommentService;
import TemaTest.services.PostService;
import TemaTest.services.UserService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class CleanupAll implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    UserService userService = new UserService();
    userService.resetIdCounter();
    userService.cleanupCSV(userService.usersCSVFileMetadata);

    PostService postService = new PostService();
    postService.resetIdCounter();
    postService.cleanupCSV(postService.postsCSVFileMetadata);

    CommentService commentService = new CommentService();
    commentService.resetIdCounter();
    commentService.cleanupCSV(commentService.commentsCSVFileMetadata);

    new Response("ok", "Cleanup finished successfully");
  }
}
