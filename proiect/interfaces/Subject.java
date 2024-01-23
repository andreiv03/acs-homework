package org.poo.cb.interfaces;

public interface Subject {
	void registerObserver(Observer observer);

	void notifyObservers();
}
