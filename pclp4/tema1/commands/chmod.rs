use std::fs::File;
use std::os::unix::fs::PermissionsExt;
use std::path::Path;

pub fn chmod(_options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.len() < 2 {
		println!("Invalid command");
		return Err(-1);
	}
	
	let mode = arguments[0].as_str();
	let file_path = Path::new(arguments[1].as_str());

	let file = match File::open(file_path) {
		Ok(file) => file,
		Err(_) => {
			return Err(-25);
		}
	};

	let metadata = match file.metadata() {
		Ok(metadata) => metadata,
		Err(_) => {
			return Err(-25)
		}
	};

	let mut permissions = metadata.permissions();
	let mut octal_code = 0o0;
	let mut operation = ' ';

	if mode.chars().all(char::is_numeric) == true {
		octal_code = match u32::from_str_radix(mode, 8) {
			Ok(octal_code) => octal_code,
			Err(_) => {
				return Err(-25)
			}
		};

		operation = '#';
	}

	else {
		let mut categories: Vec<char> = Vec::new();
		let mut symbols: Vec<char> = Vec::new();

		for character in mode.chars() {
			match character {
				'u' | 'g' | 'o' | 'a' => categories.push(character),
				'r' | 'w' | 'x' => symbols.push(character),
				'+' | '-' => operation = character,
				_ => ()
			}
		}
		
		if categories.is_empty() == true || symbols.is_empty() == true || operation == ' ' {
			println!("Invalid command");
			return Err(-1);
		}

		let mut sum = 0o0;
		
		for symbol in symbols {
			match symbol {
				'r' => sum = sum + 0o4,
				'w' => sum = sum + 0o2,
				'x' => sum = sum + 0o1,
				_ => ()
			}
		}

		for category in categories {
			match category {
				'u' => octal_code = octal_code + (sum * 0o100),
				'g' => octal_code = octal_code + (sum * 0o010),
				'o' => octal_code = octal_code + (sum * 0o001),
				'a' => octal_code = octal_code + (sum * 0o111),
				_ => ()
			}
		}
	}

	if operation == '#' {
		permissions.set_mode(octal_code);
	}

	if operation == '+' {
		permissions.set_mode(permissions.mode() | octal_code);
	}
	
	if operation == '-' {
		permissions.set_mode(permissions.mode() & !octal_code);
	}
	
	if let Err(_) = file.set_permissions(permissions) {
		return Err(-25);
	}
	
	Ok(())
}
