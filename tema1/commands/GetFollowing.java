package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.UserService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class GetFollowing implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));

    UserService userService = new UserService();
    ArrayList<String> records = userService.getFollowing(username, password);
    if (records != null)
      new Response("ok", records);
  }
}
