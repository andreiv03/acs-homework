package org.example.user;

import com.fasterxml.jackson.annotation.JsonTypeName;
import org.example.enumerations.AccountType;
import org.example.interfaces.Observer;
import org.example.interfaces.RequestsManager;
import org.example.request.Request;
import org.example.request.RequestsHolder;

@JsonTypeName("Contributor")
public class Contributor extends Staff implements RequestsManager, Observer {
	public Contributor() {
		super();
		this.setUserType(AccountType.Contributor);
	}

	public Contributor(User user) {
		super(user);
	}

	@Override
	public void createRequest(Request request) {
		RequestsHolder.addRequest(request);
	}

	@Override
	public void removeRequest(Request request) {
		RequestsHolder.removeRequest(request);
	}

	@Override
	public void receiveNotification(String notification) {
		this.getNotifications().add(notification);
	}
}
