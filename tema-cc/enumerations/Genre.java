package org.example.enumerations;

public enum Genre {
	Action,
	Adventure,
	Biography,
	Comedy,
	Cooking,
	Crime,
	Drama,
	Fantasy,
	Horror,
	Mystery,
	Romance,
	SF,
	Thriller,
	War;

	public static boolean contains(String value) {
		for (Genre genre : Genre.values())
			if (genre.name().equals(value))
				return true;

		return false;
	}
}