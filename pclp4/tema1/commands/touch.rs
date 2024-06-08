use std::fs;
use std::io::{Read, Write};
use std::path;

pub fn touch(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.is_empty() {
		println!("Invalid command");
		return Err(-1);
	}

	let mut change_accessed_time = false;
	let mut no_create = false;
	let mut change_modified_time = false;

	for option in options {
		match option.as_str() {
			"-a" => change_accessed_time = true,
			"-c" | "--no-create" => no_create = true,
			"-m" => change_modified_time = true,
			_ => ()
		}
	}
	
	let file_path = path::Path::new(arguments[0].as_str());

	if file_path.exists() == false {
		if no_create == false {
			match fs::File::create(file_path) {
				Ok(_) => (),
				Err(_) => {
					return Err(-100);
				}
			}
		}

		return Ok(());
	}

	if change_accessed_time == true {
		let mut file = match fs::OpenOptions::new().read(true).open(file_path) {
			Ok(file) => file,
			Err(_) => {
				return Err(-100);
			}
		};

		let mut content = String::new();
    let _ = file.read_to_string(&mut content);
	}

	else if change_modified_time == true {
		let mut file = match fs::OpenOptions::new().append(true).open(file_path) {
			Ok(file) => file,
			Err(_) => {
				return Err(-100);
			}
		};

		let _ = file.write(b" ");
	}

	else {
		let mut file = match fs::OpenOptions::new().read(true).append(true).open(file_path) {
			Ok(file) => file,
			Err(_) => {
				return Err(-100);
			}
		};

		let mut content = String::new();
    let _ = file.read_to_string(&mut content);
		let _ = file.write(b" ");
	}

	Ok(())
}
