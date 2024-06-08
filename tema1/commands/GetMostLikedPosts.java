package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.PostService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class GetMostLikedPosts implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));

    PostService postService = new PostService();
    String[] keys = {"post_id", "post_text", "post_date", "username", "number_of_likes"};
    ArrayList<String[]> records = postService.getMostLikedPosts(username, password);
    if (records != null)
      new Response("ok", keys, records, "b");
  }
}
