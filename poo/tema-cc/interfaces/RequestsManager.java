package org.example.interfaces;

import org.example.request.Request;

public interface RequestsManager {
	void createRequest(Request request);

	void removeRequest(Request request);
}
