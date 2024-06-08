package TemaTest.interfaces;

import TemaTest.utils.Response;

import java.util.ArrayList;

public interface Command {
  void execute(ArrayList<String> options, ArrayList<String> values);

  default boolean isAuthenticated(ArrayList<String> options) {
    if (!options.contains("-u") || !options.contains("-p")) {
      new Response("error", "You need to be authenticated");
      return false;
    }

    return true;
  }
}
