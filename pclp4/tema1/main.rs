mod commands {
	pub mod pwd;
	pub mod echo;
	pub mod cat;
	pub mod mkdir;
	pub mod mv;
	pub mod ln;
	pub mod rmdir;
	pub mod rm;
	pub mod ls;
	pub mod cp;
	pub mod touch;
	pub mod chmod;
	pub mod grep;
}

use std::collections::HashMap;
use std::env;
use std::process;

use commands::pwd::pwd;
use commands::echo::echo;
use commands::cat::cat;
use commands::mkdir::mkdir;
use commands::mv::mv;
use commands::ln::ln;
use commands::rmdir::rmdir;
use commands::rm::rm;
use commands::ls::ls;
use commands::cp::cp;
use commands::touch::touch;
use commands::chmod::chmod;
use commands::grep::grep;

fn main() {
	let command_line_arguments: Vec<String> = env::args().skip(1).collect();
	
	if command_line_arguments.len() == 0 {
		println!("Invalid command");
		process::exit(-1);
	}

	let command = command_line_arguments[0].as_str();
	let mut commands: HashMap<&str, fn(&Vec<String>, &Vec<String>) -> Result<(), i32>> = HashMap::new();
	commands.insert("pwd", pwd);
	commands.insert("echo", echo);
	commands.insert("cat", cat);
	commands.insert("mkdir", mkdir);
	commands.insert("mv", mv);
	commands.insert("ln", ln);
	commands.insert("rmdir", rmdir);
	commands.insert("rm", rm);
	commands.insert("ls", ls);
	commands.insert("cp", cp);
	commands.insert("touch", touch);
	commands.insert("chmod", chmod);
	commands.insert("grep", grep);

	if let Some(execute_command) = commands.get(command) {
		let mut options: Vec<String> = Vec::new();
		let mut arguments: Vec<String> = Vec::new();

		for argument in command_line_arguments.iter().skip(1) {
			match argument {
				_ if argument.starts_with("-") || argument.starts_with("--") => {
					options.push(argument.to_string());
				},
				_ => {
					arguments.push(argument.to_string());
				}
			}
		}

		match execute_command(&options, &arguments) {
			Ok(_) => {
				process::exit(0);
			},
			Err(error) => {
				process::exit(error);
			}
		}
	}

	println!("Invalid command");
	process::exit(-1);
}
