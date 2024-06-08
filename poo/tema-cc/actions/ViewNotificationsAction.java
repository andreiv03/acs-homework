package org.example.actions;

import org.example.user.User;

public class ViewNotificationsAction extends Action {
	@Override
	public void execute(User currentUser) {
		try {
			System.out.println("Notifications:");

			if (currentUser.getNotifications() == null || currentUser.getNotifications().isEmpty()) {
				System.out.println("You have no notifications!");
				System.out.println();
				return;
			}

			for (String notification : currentUser.getNotifications())
				System.out.println((currentUser.getNotifications().indexOf(notification) + 1) + ") " + notification);

			System.out.println();
		} catch (Exception ignored) {
		}
	}
}
