package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.UserService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class GetFollowers implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String targetUsername = options.contains("-username") ? values.get(options.indexOf("-username")) : null;

    UserService userService = new UserService();
    ArrayList<String> records = userService.getFollowers(username, password, targetUsername);
    if (records != null)
      new Response("ok", records);
  }
}
