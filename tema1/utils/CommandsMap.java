package TemaTest.utils;

import TemaTest.interfaces.Command;

import java.util.HashMap;
import java.util.Map;

public class CommandsMap {
  private final Map<String, Command> commandsMap;

  public CommandsMap() {
    this.commandsMap = new HashMap<>();
  }

  public Command get(String commandName) {
    return commandsMap.get(commandName);
  }

  public void add(String commandName, Command command) {
    commandsMap.put(commandName, command);
  }
}
