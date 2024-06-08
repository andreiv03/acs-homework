extern crate regex;

use regex::Regex;
use std::fs;

pub fn grep(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.len() < 2 {
		println!("Invalid command");
		return Err(-1);
	}

	let mut is_invert_match = false;

	for option in options {
		match option.as_str() {
			"-i" | "-v" | "--invert-match" => is_invert_match = true,
			_ => ()
		}
	}

	let pattern = arguments[0].as_str();
	let content = match fs::read_to_string(arguments[1].as_str()) {
		Ok(content) => content,
		Err(_) => {
			return Err(-1);
		}
	};

	let regex = match Regex::new(pattern) {
		Ok(regex) => regex,
		Err(_) => {
			return Err(-1);
		}
	};

	for line in content.lines() {
		if is_invert_match == true {
			if regex.is_match(line) == false {
				println!("{}", line);
			}
		}

		if is_invert_match == false {
			if regex.is_match(line) == true {
				println!("{}", line);
			}
		}
	}

	Ok(())
}
