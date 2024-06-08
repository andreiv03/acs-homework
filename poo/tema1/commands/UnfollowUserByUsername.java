package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.UserService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class UnfollowUserByUsername implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!isAuthenticated(options))
      return;

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));
    String followingUsername = options.contains("-username") ? values.get(options.indexOf("-username")) : null;

    UserService userService = new UserService();
    if (userService.followUser(username, password, followingUsername, false))
      new Response("ok", "Operation executed successfully");
  }
}
