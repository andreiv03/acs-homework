#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void readFromInputFile(char *fileName, int *inputsNumber, int *outputIndex, Gate *gates, int *gatesNumber) {
	FILE *file = fopen(fileName, "r");

	if (file == NULL) {
		printf("Could not open input file: %s\n", fileName);
		exit(1);
	}

	fscanf(file, "%d %d", inputsNumber, outputIndex);

	while (1) {
		Gate gate;
		gate.inputs.number = 0;
		gate.inputs.indexes = NULL;

		char type[4];
		fscanf(file, "%s", type);

		if (strcmp(type, "AND") == 0)
			gate.type = AND;

		else if (strcmp(type, "OR") == 0)
			gate.type = OR;

		else if (strcmp(type, "NOT") == 0)
			gate.type = NOT;

		int input = 0;

		while (fscanf(file, "%d", &input) == 1) {
			gate.inputs.number = gate.inputs.number + 1;
			gate.inputs.indexes = realloc(gate.inputs.indexes, gate.inputs.number * sizeof(int));

			if (gate.inputs.indexes == NULL) {
				for (int gateIndex = 0; gateIndex < *gatesNumber; ++gateIndex)
					free(gates[gateIndex].inputs.indexes);

				fclose(file);
				exit(1);
			}

			gate.inputs.indexes[gate.inputs.number - 1] = input;
		}

		gate.inputs.number = gate.inputs.number - 1;
		gate.outputIndex = gate.inputs.indexes[gate.inputs.number];
		gate.inputs.indexes = realloc(gate.inputs.indexes, gate.inputs.number * sizeof(int));

		gates[*gatesNumber] = gate;
		*gatesNumber = *gatesNumber + 1;

		if (gate.outputIndex == *outputIndex)
			break;
	}

	fclose(file);
}

void addClause(Clause *clauses, int *clausesNumber, const int *literals, int literalsNumber) {
	clauses[*clausesNumber].literals = calloc(literalsNumber, sizeof(int));

	if (clauses[*clausesNumber].literals == NULL) {
		for (int clauseIndex = 0; clauseIndex < *clausesNumber; ++clauseIndex)
			free(clauses[clauseIndex].literals);

		exit(1);
	}

	clauses[*clausesNumber].number = literalsNumber;

	for (int literalIndex = 0; literalIndex < literalsNumber; ++literalIndex)
		clauses[*clausesNumber].literals[literalIndex] = literals[literalIndex];

	*clausesNumber = *clausesNumber + 1;
}

void determineClausesFromGates(Gate *gates, int gatesNumber, Clause *clauses, int *clausesNumber) {
	for (int gateIndex = 0; gateIndex < gatesNumber; ++gateIndex) {
		Gate gate = gates[gateIndex];

		int literals[gate.inputs.number + 1];
		int literalsNumber = 0;

		switch (gate.type) {
			case AND:
				literals[literalsNumber++] = gate.outputIndex;
				literals[literalsNumber++] = -gate.outputIndex;
				addClause(clauses, clausesNumber, literals, literalsNumber);

				for (int inputIndex = 0; inputIndex < gate.inputs.number; ++inputIndex) {
					literalsNumber = 0;
					literals[literalsNumber++] = gate.inputs.indexes[inputIndex];
					literals[literalsNumber++] = -gate.outputIndex;
					addClause(clauses, clausesNumber, literals, literalsNumber);
				}

				break;

			case OR:
				for (int inputIndex = 0; inputIndex < gate.inputs.number; ++inputIndex)
					literals[literalsNumber++] = gate.inputs.indexes[inputIndex];

				literals[literalsNumber++] = -gate.outputIndex;
				addClause(clauses, clausesNumber, literals, literalsNumber);

				for (int inputIndex = 0; inputIndex < gate.inputs.number; ++inputIndex) {
					literalsNumber = 0;
					literals[literalsNumber++] = -gate.inputs.indexes[inputIndex];
					literals[literalsNumber++] = gate.outputIndex;
					addClause(clauses, clausesNumber, literals, literalsNumber);
				}

				break;

			case NOT:
				literals[literalsNumber++] = gate.inputs.indexes[0];
				literals[literalsNumber++] = gate.outputIndex;
				addClause(clauses, clausesNumber, literals, literalsNumber);

				literalsNumber = 0;
				literals[literalsNumber++] = -gate.inputs.indexes[0];
				literals[literalsNumber++] = -gate.outputIndex;
				addClause(clauses, clausesNumber, literals, literalsNumber);
				break;
		}
	}
}

void writeToOutputFile(char *fileName, int outputIndex, Clause *clauses, int clausesNumber) {
	FILE *file = fopen(fileName, "w");

	if (file == NULL) {
		printf("Could not open output file: %s\n", fileName);
		exit(1);
	}

	fprintf(file, "p cnf %d %d\n", outputIndex, clausesNumber + 1);

	for (int clauseIndex = 0; clauseIndex < clausesNumber; ++clauseIndex) {
		Clause clause = clauses[clauseIndex];

		for (int literalIndex = 0; literalIndex < clause.number; ++literalIndex)
			fprintf(file, "%d ", clause.literals[literalIndex]);

		fprintf(file, "0\n");
	}

	fprintf(file, "%d 0\n", outputIndex);
	fclose(file);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <inputFile> <outputFile>\n", argv[0]);
		exit(1);
	}

	int inputsNumber = 0;
	int outputIndex = 0;

	Gate gates[100];
	int gatesNumber = 0;

	Clause clauses[1000];
	int clausesNumber = 0;

	readFromInputFile(argv[1], &inputsNumber, &outputIndex, gates, &gatesNumber);
	determineClausesFromGates(gates, gatesNumber, clauses, &clausesNumber);
	writeToOutputFile(argv[2], outputIndex, clauses, clausesNumber);

	for (int gateIndex = 0; gateIndex < gatesNumber; ++gateIndex)
		free(gates[gateIndex].inputs.indexes);

	for (int clauseIndex = 0; clauseIndex < clausesNumber; ++clauseIndex)
		free(clauses[clauseIndex].literals);

	return 0;
}
