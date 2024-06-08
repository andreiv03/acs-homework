package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.PostService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class UnlikePost implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String postId = options.contains("-post-id") ? values.get(options.indexOf("-post-id")) : null;

    PostService postService = new PostService();
    if (postService.likePost(username, password, postId, false))
      new Response("ok", "Operation executed successfully");
  }
}
