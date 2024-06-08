use std::env;

pub fn pwd(_options: &Vec<String>, _arguments: &Vec<String>) -> Result<(), i32> {
	let path = match env::current_dir() {
		Ok(path) => path,
		Err(_) => {
			return Err(-1);
		}
	};

	println!("{}", path.display());
	
	Ok(())
}
