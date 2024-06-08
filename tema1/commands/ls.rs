extern crate chrono;

use chrono::Local;
use std::fs::{DirEntry, read_dir, read_to_string};
use std::os::unix::fs::PermissionsExt;
use std::os::linux::fs::MetadataExt;
use std::path;

fn print_long_format(entry: &DirEntry) -> Result<(), i32> {
	let mut output = String::new();

	let file_type = match entry.file_type() {
		Ok(file_type) => file_type,
		Err(_) => {
			return Err(-80);
		}
	};

	match file_type {
		file_type if file_type.is_dir() => output.push_str("d"),
		_ => output.push_str("-")
	}

	let metadata = match entry.metadata() {
		Ok(metadata) => metadata,
		Err(_) => {
			return Err(-80);
		}
	};

	let mode = metadata.permissions().mode();

	for index in 0..9 {
		let permission = (mode >> (8 - index)) & 1;
    	let symbol = match permission {
			1 => match index % 3 {
				0 => "r",
				1 => "w",
				2 => "x",
				_ => "-"
			},
			_ => "-"
		};

   		output.push_str(symbol);
	}
	
	output.push_str(" ");

	let passwd_file_content = match read_to_string("/etc/passwd") {
		Ok(passwd_file_content) => passwd_file_content,
		Err(_) => {
			return Err(-80);
		}
	};

	for line in passwd_file_content.lines() {
		let fields: Vec<&str> = line.split(":").collect();

		if fields.len() > 3 && fields[2] == metadata.st_uid().to_string() {
			output.push_str(fields[0]);
			output.push_str(" ");
			break;
		}
	}

	let group_file_content = match read_to_string("/etc/group") {
		Ok(group_file_content) => group_file_content,
		Err(_) => {
			return Err(-80);
		}
	};

	for line in group_file_content.lines() {
		let fields: Vec<&str> = line.split(":").collect();

		if fields.len() > 3 && fields[2] == metadata.st_gid().to_string() {
			output.push_str(fields[0]);
			output.push_str(" ");
			break;
		}
	}

	output.push_str(&metadata.len().to_string());
	output.push_str(" ");

	let modified_time = match metadata.modified() {
		Ok(time) => time,
		Err(_) => {
			return Err(-80);
		}
	};

	let date_time: chrono::DateTime<Local> = modified_time.into();
  let formatted_time = date_time.format("%e %H:%M").to_string();
	output.push_str(&formatted_time.trim());

	print!("{} ", output);

	Ok(())
}

fn list_recursive(path: &path::Path, is_hidden: bool, is_long: bool) -> Result<(), i32> {
	let mut directories: Vec<String> = Vec::new();

	println!("{}:", path.display());

	if is_hidden == true {
		println!(".");
		println!("..");
	}

	let entries = match read_dir(path) {
		Ok(entries) => entries,
		Err(_) => {
			return Err(-80);
		}
	};

	for entry in entries {
		let entry = match entry {
			Ok(entry) => entry,
			Err(_) => {
				return Err(-80);
			}
		};

		let file_name = entry.file_name().to_string_lossy().to_string();
		let file_type = match entry.file_type() {
			Ok(file_type) => file_type,
			Err(_) => {
				return Err(-80);
			}
		};
		
		if is_hidden == false && file_name.starts_with(".") {
			continue;
		}

		if file_type.is_dir() {
			directories.push(file_name.clone());
		}

		if is_long == true {
			let _ = print_long_format(&entry);
		}

		println!("{}", file_name);
	}

	for directory in directories {
		println!("");

		let new_path = path.join(directory);
		
		if let Err(error) = list_recursive(&new_path, is_hidden, is_long) {
			return Err(error);
		}
	}

	Ok(())
}

pub fn ls(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	let mut is_hidden = false;
	let mut is_long = false;
	let mut is_recursive = false;

	for option in options {
		match option.as_str() {
			"-a" | "--all" => is_hidden = true,
			"-l" => is_long = true,
			"-R" | "--recursive" => is_recursive = true,
			_ => ()
		}
	}

	let path = if arguments.len() == 0 {
		path::Path::new(".")
	} else {
		path::Path::new(arguments[0].as_str())
	};

	if path.exists() == false {
		return Err(-80);
	}

	if path.is_file() {
		println!("{}", path.display());
	}
	
	if path.is_dir() {
		if is_recursive == true {
			match list_recursive(&path, is_hidden, is_long) {
				Ok(_) => (),
				Err(error) => {
					return Err(error);
				}
			}
		}

		if is_recursive == false {
			if is_hidden == true {
				println!(".");
				println!("..");
			}

			let entries = match read_dir(path) {
				Ok(entries) => entries,
				Err(_) => {
					return Err(-80);
				}
			};

			for entry in entries {
				let entry = match entry {
					Ok(entry) => entry,
					Err(_) => {
						return Err(-80);
					}
				};

				let file_name = entry.file_name().to_string_lossy().to_string();

				if is_hidden == false && file_name.starts_with(".") {
					continue;
				}

				if is_long == true {
					let _ = print_long_format(&entry);
				}

				println!("{}", file_name);
			}
		}
	}

	Ok(())
}
