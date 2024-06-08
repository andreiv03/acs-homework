package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.PostService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class DeletePostById implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String postId = options.contains("-id") ? values.get(options.indexOf("-id")) : null;

    PostService postService = new PostService();
    if (postService.deletePost(username, password, postId))
      new Response("ok", "Post deleted successfully");
  }
}
