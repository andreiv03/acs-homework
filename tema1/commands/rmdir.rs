use std::fs;

pub fn rmdir(_options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.is_empty() {
		println!("Invalid command");
		return Err(-1);
	}

	for argument in arguments {
		match fs::remove_dir(argument) {
			Ok(_) => (),
			Err(_) => {
				return Err(-60);
			}
		}
	}
	
	Ok(())
}
