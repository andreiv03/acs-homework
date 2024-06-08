package org.poo.cb.serializers;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;

import java.io.IOException;
import java.text.DecimalFormat;

public class CustomDoubleSerializer extends StdSerializer<Double> {
	public CustomDoubleSerializer() {
		super(Double.class);
	}

	@Override
	public void serialize(Double value, JsonGenerator jsonGenerator, SerializerProvider serializerProvider) throws IOException {
		jsonGenerator.writeString(new DecimalFormat("0.00").format(value));
	}
}
