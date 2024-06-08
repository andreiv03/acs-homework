#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

void get_operations(void **operations);

void readSensorsData(FILE *fileStream, sensor **sensors, int sensorsLength);
void checkPriority(sensor **sensors, int sensorsLength);

void printCommand(sensor **sensors, int sensorsLength, int sensorIndex);
void analyzeCommand(sensor **sensors, int sensorsLength, int sensorIndex);
void clearCommand(sensor **sensors, int *sensorsLength);
void exitCommand(sensor **sensors, int sensorsLength);

int main(int argc, char const *argv[]) {
	if (argc == 1)
		return 1;

	FILE *inputFileStream = fopen(argv[1], "rb");
	if (inputFileStream == NULL)
		return 1;

	int sensorsLength = 0;
	fread(&sensorsLength, sizeof(int), 1, inputFileStream);
	sensor **sensors = calloc(sensorsLength, sizeof(sensor *));
	readSensorsData(inputFileStream, sensors, sensorsLength);

	char buffer[256];
	while (1) {
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			break;

		char command[256];
		int index = 0;
		sscanf(buffer, "%s %d", command, &index);

		if (strcmp(command, "print") == 0)
			printCommand(sensors, sensorsLength, index);

		if (strcmp(command, "analyze") == 0)
			analyzeCommand(sensors, sensorsLength, index);

		if (strcmp(command, "clear") == 0)
			clearCommand(sensors, &sensorsLength);

		if (strcmp(command, "exit") == 0) {
			exitCommand(sensors, sensorsLength);
			break;
		}
	}

	return 0;
}

void readSensorsData(FILE *fileStream, sensor **sensors, int sensorsLength) {
	for (int index = 0; index < sensorsLength; ++index) {
		sensors[index] = calloc(1, sizeof(sensor));
		fread(&sensors[index]->sensor_type, sizeof(int), 1, fileStream);

		if (sensors[index]->sensor_type == 0) {
			sensors[index]->sensor_data = calloc(1, sizeof(tire_sensor));
			tire_sensor *data = sensors[index]->sensor_data;
			fread(&data->pressure, sizeof(float), 1, fileStream);
			fread(&data->temperature, sizeof(float), 1, fileStream);
			fread(&data->wear_level, sizeof(int), 1, fileStream);
			fread(&data->performace_score, sizeof(int), 1, fileStream);
		}

		if (sensors[index]->sensor_type == 1) {
			sensors[index]->sensor_data = calloc(1, sizeof(power_management_unit));
			power_management_unit *data = sensors[index]->sensor_data;
			fread(&data->voltage, sizeof(float), 1, fileStream);
			fread(&data->current, sizeof(float), 1, fileStream);
			fread(&data->power_consumption, sizeof(float), 1, fileStream);
			fread(&data->energy_regen, sizeof(int), 1, fileStream);
			fread(&data->energy_storage, sizeof(int), 1, fileStream);
		}

		fread(&sensors[index]->nr_operations, sizeof(int), 1, fileStream);
		int operationsLength = sensors[index]->nr_operations;
		sensors[index]->operations_idxs = calloc(operationsLength, sizeof(int));
		fread(sensors[index]->operations_idxs, sizeof(int), operationsLength, fileStream);

		checkPriority(sensors, index);
	}

	fclose(fileStream);
}

void checkPriority(sensor **sensors, int lastSensorIndex) {
	if (sensors[lastSensorIndex]->sensor_type == 0)
		return;

	if (lastSensorIndex == 0)
		return;

	int firstTireIndex = 0;
	while (sensors[firstTireIndex] && sensors[firstTireIndex]->sensor_type == 1)
		firstTireIndex = firstTireIndex + 1;

	if (firstTireIndex > lastSensorIndex)
		return;

	while (firstTireIndex != lastSensorIndex) {
		void *temp = sensors[lastSensorIndex];
		sensors[lastSensorIndex] = sensors[lastSensorIndex - 1];
		sensors[lastSensorIndex - 1] = temp;
		lastSensorIndex = lastSensorIndex - 1;
	}
}

void printCommand(sensor **sensors, int sensorsLength, int sensorIndex) {
	if (sensorIndex >= sensorsLength || sensorIndex < 0) {
		printf("Index not in range!\n");
		return;
	}

	if (sensors[sensorIndex]->sensor_type == 0) {
		printf("Tire Sensor\n");
		tire_sensor *data = sensors[sensorIndex]->sensor_data;
		printf("Pressure: %.2f\n", data->pressure);
		printf("Temperature: %.2f\n", data->temperature);
		printf("Wear Level: %d%%\n", data->wear_level);
		if (data->performace_score == 0)
			printf("Performance Score: Not Calculated\n");
		else
			printf("Performance Score: %d\n", data->performace_score);
	}

	if (sensors[sensorIndex]->sensor_type == 1) {
		printf("Power Management Unit\n");
		power_management_unit *data = sensors[sensorIndex]->sensor_data;
		printf("Voltage: %.2f\n", data->voltage);
		printf("Current: %.2f\n", data->current);
		printf("Power Consumption: %.2f\n", data->power_consumption);
		printf("Energy Regen: %d%%\n", data->energy_regen);
		printf("Energy Storage: %d%%\n", data->energy_storage);
	}
}

void analyzeCommand(sensor **sensors, int sensorsLength, int sensorIndex) {
	if (sensorIndex >= sensorsLength || sensorIndex < 0) {
		printf("Index not in range!\n");
		return;
	}

	void (**operations)(void *) = calloc(8, sizeof(void (*)(void *)));
	get_operations((void **)operations);

	for (int index = 0; index < sensors[sensorIndex]->nr_operations; ++index) {
		int operationIndex = sensors[sensorIndex]->operations_idxs[index];
		operations[operationIndex](sensors[sensorIndex]->sensor_data);
	}

	free(operations);
}

void clearCommand(sensor **sensors, int *sensorsLength) {
	for (int index = 0; index < *sensorsLength; ++index) {
		int isErroneous = 0;

		if (sensors[index]->sensor_type == 0) {
			tire_sensor *data = sensors[index]->sensor_data;

			if (data->pressure < 19 || data->pressure > 28)
				isErroneous = 1;

			if (data->temperature < 0 || data->temperature > 120)
				isErroneous = 1;

			if (data->wear_level < 0 || data->wear_level > 100)
				isErroneous = 1;
		}

		if (sensors[index]->sensor_type == 1) {
			power_management_unit *data = sensors[index]->sensor_data;

			if (data->voltage < 10 || data->voltage > 20)
				isErroneous = 1;

			if (data->current < -100 || data->current > 100)
				isErroneous = 1;

			if (data->power_consumption < 0 || data->power_consumption > 1000)
				isErroneous = 1;

			if (data->energy_regen < 0 || data->energy_regen > 100)
				isErroneous = 1;

			if (data->energy_storage < 0 || data->energy_storage > 100)
				isErroneous = 1;
		}

		if (isErroneous == 0)
			continue;

		void *temp = sensors[index];
		for (int tempIndex = index; tempIndex < *sensorsLength - 1; ++tempIndex)
			sensors[tempIndex] = sensors[tempIndex + 1];

		*sensorsLength = *sensorsLength - 1;
		index = index - 1;

		free(((sensor *)temp)->sensor_data);
		free(((sensor *)temp)->operations_idxs);
		free(((sensor *)temp));
	}
}

void exitCommand(sensor **sensors, int sensorsLength) {
	for (int index = 0; index < sensorsLength; ++index) {
		free(sensors[index]->sensor_data);
		free(sensors[index]->operations_idxs);
		free(sensors[index]);
	}

	free(sensors);
}
