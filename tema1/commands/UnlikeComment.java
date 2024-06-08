package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.CommentService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class UnlikeComment implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String commentId = options.contains("-comment-id") ? values.get(options.indexOf("-comment-id")) : null;

    CommentService commentService = new CommentService();
    if (commentService.likeComment(username, password, commentId, false))
      new Response("ok", "Operation executed successfully");
  }
}
