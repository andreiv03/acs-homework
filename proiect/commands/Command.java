package org.poo.cb.commands;

import java.util.ArrayList;

public abstract class Command {
	public abstract void execute(ArrayList<String> arguments) throws Exception;
}
