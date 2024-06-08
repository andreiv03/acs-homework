use std::fs;
use std::path;

fn copy_recursive(source: &path::Path, destination: &path::Path) -> Result<(), i32> {
	if source.is_dir() {
		if destination.exists() == false {
			match fs::create_dir(destination) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		}

		let entries = match fs::read_dir(source) {
			Ok(entries) => entries,
			Err(_) => {
				return Err(-90);
			}
		};

		for entry in entries {
			let entry = match entry {
				Ok(entry) => entry,
				Err(_) => {
					return Err(-90);
				}
			};

			let mut path = entry.path();
			path = source.join(path.file_name().unwrap());

			if path.is_dir() {
				match copy_recursive(&path, &destination.join(path.file_name().unwrap())) {
					Ok(_) => (),
					Err(_) => {
						return Err(-90);
					}
				}
			} else {
				let _path_file_name = match path.file_name() {
					Some(path_file_name) => path_file_name,
					None => {
						return Err(-90);
					}
				};

				if destination.is_dir() == true {
					if destination.join(source).exists() == false {
						match fs::create_dir(&destination.join(source)) {
							Ok(_) => (),
							Err(_) => {
								return Err(-90);
							}
						}
					}

					match fs::copy(&path, &destination.join(&path)) {
						Ok(_) => (),
						Err(_) => {
							return Err(-90);
						}
					}
				} else {
					match fs::copy(&path, &destination) {
						Ok(_) => (),
						Err(_) => {
							return Err(-90);
						}
					}
				}
			}
		}
	} else {
		if destination.exists() == false {
			match fs::create_dir(destination) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		}

		let source_file_name = match source.file_name() {
			Some(source_file_name) => source_file_name,
			None => {
				return Err(-90);
			}
		};

		if destination.is_dir() == true {
			match fs::copy(source, &destination.join(source_file_name)) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		} else {
			match fs::copy(source, destination) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		}
	}

	Ok(())
}

pub fn cp(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.len() < 2 {
		println!("Invalid command");
		return Err(-1);
	}
	
	let mut is_recursive = false;

	for option in options {
		match option.as_str() {
			"-r" | "-R" | "--recursive" => is_recursive = true,
			_ => ()
		}
	}

	let source = path::Path::new(arguments[0].as_str());
	let destination = path::Path::new(arguments[1].as_str());

	let source_file_name = match source.file_name() {
		Some(source_file_name) => source_file_name,
		None => {
			return Err(-90);
		}
	};

	if is_recursive == true {
		match copy_recursive(&source, &destination) {
			Ok(_) => (),
			Err(_) => {
				return Err(-90);
			}
		}
	} else {
		if destination.is_dir() == true {
			match fs::copy(source, &destination.join(source_file_name)) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		} else {
			match fs::copy(source, destination) {
				Ok(_) => (),
				Err(_) => {
					return Err(-90);
				}
			}
		}
	}

	Ok(())
}
