use std::fs;
use std::io::Read;

pub fn cat(_options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.is_empty() {
		println!("Invalid command");
		return Err(-1);
	}

	for argument in arguments {
		let mut file = match fs::File::open(argument) {
			Ok(file) => file,
			Err(_) => {
				return Err(-20);
			}
		};

		let mut content = String::new();

		if let Err(_) = file.read_to_string(&mut content) {
			return Err(-20);
		}

		print!("{}", content);
	}

	Ok(())
}
