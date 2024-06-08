In utils.h se afla toate structurile folosite in program.

Mai intai, creez un array de gates in care retin toate portile din circuit.
Pentru fiecare gate din array, aplic subexpresiile corespunzatoare tipului acesteia din transformarea Tseytin.
Astfel, determin clauzele corespunzatoare fiecarei porti.

Pentru fiecare tip de gate, am urmat urmatoarele reguli generale:
AND: A and B = C -> (A or not C) and (B or not C) and (not A or not B or C)
OR: A or B = C -> (not A or C) and (not B or C) and (A or B or not C)
NOT: not A = B -> (A or B) and (not A or not B)

In final, afisez numarul de variabile (care este index-ul output-ului circuitului initial) si numarul de clauze, apoi clauzele.
