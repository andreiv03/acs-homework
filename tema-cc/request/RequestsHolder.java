package org.example.request;

import java.util.ArrayList;

public final class RequestsHolder {
	private static final ArrayList<Request> requests = new ArrayList<>();

	public static ArrayList<Request> getRequests() {
		return requests;
	}

	public static void addRequest(Request request) {
		requests.add(request);
	}

	public static void removeRequest(Request request) {
		requests.remove(request);
	}
}
