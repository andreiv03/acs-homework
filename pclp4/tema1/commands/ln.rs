use std::fs::hard_link;
use std::os::unix::fs::symlink;
use std::path;

pub fn ln(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.len() < 2 {
		println!("Invalid command");
		return Err(-1);
	}

	let mut is_symbolic = false;

	for option in options {
		match option.as_str() {
			"-s" | "--symbolic" => is_symbolic = true,
			_ => ()
		}
	}

	let source = path::Path::new(arguments[0].as_str());
	let destination = path::Path::new(arguments[1].as_str());

	if source == destination {
		println!("Invalid command");
		return Err(-1);
	}

	if source.is_dir() {
		return Err(-50);
	}

	if is_symbolic == true {
		match symlink(source, destination) {
			Ok(_) => (),
			Err(_) => {
				return Err(-50);
			}
		}
	}
	
	if is_symbolic == false {
		match hard_link(source, destination) {
			Ok(_) => (),
			Err(_) => {
				return Err(-50);
			}
		}
	}

	Ok(())
}
