#ifndef UTILS_H
#define UTILS_H

typedef enum { AND, OR, NOT } GateType;

typedef struct GateInput {
	int *indexes;
	int number;
} GateInput;

typedef struct Gate {
	GateType type;
	GateInput inputs;
	int outputIndex;
} Gate;

typedef struct Clause {
	int *literals;
	int number;
} Clause;

#endif
