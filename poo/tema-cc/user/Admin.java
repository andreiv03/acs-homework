package org.example.user;

import com.fasterxml.jackson.annotation.JsonTypeName;
import org.example.enumerations.AccountType;
import org.example.interfaces.Observer;

@JsonTypeName("Admin")
public class Admin extends Staff implements Observer {
	public Admin() {
		super();
		this.setUserType(AccountType.Admin);
	}

	public Admin(User user) {
		super(user);
	}

	@Override
	public void receiveNotification(String notification) {
		this.getNotifications().add(notification);
	}
}
