use std::path;
use std::fs;

pub fn rm(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.is_empty() {
		println!("Invalid command");
		return Err(-1);
	}

	let mut remove_empty_directories = false;
	let mut is_recursive = false;

	for option in options {
		match option.as_str() {
			"-d" | "--dir" => remove_empty_directories = true,
			"-r" | "-R" | "--recursive" => is_recursive = true,
			_ => ()
		}
	}

	let mut caught_error = false;

	for argument in arguments {
		let path = path::Path::new(argument);

		if path.exists() == false {
			caught_error = true;
			continue;
		}

		if path.is_file() {
			match fs::remove_file(path) {
				Ok(_) => (),
				Err(_) => {
					caught_error = true;
					continue;
				}
			}
		}

		if path.is_dir() {
			if is_recursive == true {
				match fs::remove_dir_all(path) {
					Ok(_) => (),
					Err(_) => {
						caught_error = true;
						continue;
					}
				}
			}
			
			else if remove_empty_directories == true {
				match fs::remove_dir(path) {
					Ok(_) => (),
					Err(_) => {
						caught_error = true;
						continue;
					}
				}
			}

			else {
				caught_error = true;
				continue;
			}
		}
	}

	if caught_error == true {
		return Err(-70);
	}

	Ok(())
}
