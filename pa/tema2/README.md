Task 1: Utilizand programarea dinamica, am determinat numarul de drumuri comune de la nodul 1 la nodul n din doua grafuri intersectate. Dupa citirea celor doua grafuri, am calculat drumurile pentru fiecare nod si am actualizat numarul de drumuri catre destinatie.
Complexitate: O(N*M)

Task 2: Am determinat numarul maxim de orase vizitate de la un oras de start la un oras destinatie folosind rute directe. Printr-o abordare DFS, am utilizat sortarea topologica pentru a calcula drumul maxim de la nodul de start la nodul de destinatie intr-un graf neorientat.
Complexitate: O(N+M)

Task 3: Folosind algoritmul Dijkstra, am gasit distantele minime de la nodurile x, y si z la toate celelalte noduri. Am ales submultimea de muchii care minimizeaza suma distantelor, actualizand vectorii de distante pentru a obtine solutia optima.
Complexitate: O((N+M)*logN)

Task 4: Am citit grafurile din fisier si am construit liste de adiacenta pentru relatii si conflicte. Am folosit sortarea topologica si algoritmul de componente tari conexe pentru a determina care prieteni pot fi invitati fara a incalca regulile, rezultand intr-o lista optima de invitati.
Complexitate: O(N+M).
