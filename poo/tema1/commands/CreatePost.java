package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.PostService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class CreatePost implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String text = options.contains("-text") ? values.get(options.indexOf("-text")) : null;

    PostService postService = new PostService();
    if (postService.createPost(username, password, text))
      new Response("ok", "Post added successfully");
  }
}
