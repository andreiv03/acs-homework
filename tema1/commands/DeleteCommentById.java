package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.CommentService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class DeleteCommentById implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String commentId = options.contains("-id") ? values.get(options.indexOf("-id")) : null;

    CommentService commentService = new CommentService();
    if (commentService.deleteComment(username, password, commentId))
      new Response("ok", "Operation executed successfully");
  }
}
