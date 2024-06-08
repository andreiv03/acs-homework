package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.PostService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class GetUserPosts implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String followingUsername = options.contains("-username") ? values.get(options.indexOf("-username")) : null;

    PostService postService = new PostService();
    String[] keys = {"post_id", "post_text", "post_date"};
    ArrayList<String[]> records = postService.getUserPosts(username, password, followingUsername);
    if (records != null)
      new Response("ok", keys, records);
  }
}
