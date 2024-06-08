package TemaTest.commands;

import TemaTest.interfaces.Command;
import TemaTest.services.UserService;
import TemaTest.utils.Response;

import java.util.ArrayList;

public class CreateUser implements Command {
  @Override
  public void execute(ArrayList<String> options, ArrayList<String> values) {
    if (!options.contains("-u")) {
      new Response("error", "Please provide username");
      return;
    }

    if (!options.contains("-p")) {
      new Response("error", "Please provide password");
      return;
    }

    String username = values.get(options.indexOf("-u"));
    String password = values.get(options.indexOf("-p"));

    UserService userService = new UserService();
    if (userService.createUser(username, password))
      new Response("ok", "User created successfully");
  }
}
