use std::fs;
use std::path;

pub fn mv(_options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.len() < 2 {
		println!("Invalid command");
		return Err(-1);
	}
	
	let source = path::Path::new(arguments[0].as_str());
	let destination = path::Path::new(arguments[1].as_str());

	match fs::rename(source, destination) {
		Ok(_) => (),
		Err(_) => {
			return Err(-40);
		}
	}
	
	Ok(())
}
