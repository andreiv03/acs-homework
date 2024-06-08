package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.CommentService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class CommentPost implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String postId = options.contains("-post-id") ? values.get(options.indexOf("-post-id")) : null;
    String text = options.contains("-text") ? values.get(options.indexOf("-text")) : null;

    CommentService commentService = new CommentService();
    if (commentService.createComment(username, password, postId, text))
      new Response("ok", "Comment added successfully");
  }
}
