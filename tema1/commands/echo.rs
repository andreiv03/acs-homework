pub fn echo(options: &Vec<String>, arguments: &Vec<String>) -> Result<(), i32> {
	if arguments.is_empty() == true {
		println!("Invalid command");
		return Err(-1);
	}

	let mut contains_newline = true;

	for option in options {
		match option.as_str() {
			"-n" => contains_newline = false,
			_ => ()
		}
	}

	for (index, argument) in arguments.iter().enumerate() {
		print!("{}", argument);

		if index < arguments.len() - 1 {
			print!(" ");
		}
	}

	if contains_newline == true {
		print!("\n");
	}

	Ok(())
}
