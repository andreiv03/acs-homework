# Markos is coming

- parse_labyirinth.m: Citesc dimensiunile matricei, apoi matricea pe care o transpun pentru a corecta ordinea elementelor.
- get_adjacency_matrix.m: Folosesc doi vectori de pozitii pentru coordonatele i si j pentru a ma deplasa pe vecinii unui nod.
- get_link_matrix.m: Functie identica cu get_adjacency_matrix.m
- get_Jacobi_parameters.m: Extrag parametrii din matricea de legaturi.
- perform-iterative.m: Am implementat metoda Jacobi.
- heuristic_greedy.m: Am implementat pseudocodul dat.
- decode_path.m: Determin coordonatele fiecarui nod si le introduc intr-o matrice de doua coloane.

# Linear Regression

- parse_data_set_file.m: Citesc dimensiunile matricei, apoi primul element de pe linie pe care il introduc in Y si restul liniei pe care o introduc in matrice.
- prepare_for_regression.m: Parcurg matricea si o transform conform cerintei.
- parse_csv_file.m: Functie aproape identica cu parse_data_set_file.m
- linear_regression_cost_function.m: Aplic algoritmul descris in cerinta.
- gradient_descedent.m: Aplic algoritmul descris in cerinta.
- normal_equation.m: Aplic algoritmul descris in cerinta.
- lasso_regression_cost_function.m: Aplic algoritmul descris in cerinta.
- ridge_regression_cost_function.m: Aplic algoritmul descris in cerinta.

# MNIST 101

- load_dataset.m: Incarc fisierul si extrag X si y.
- split_dataset.m: Divizez parametrii X si y in functie de procentul dat.
- initialize_weights.m: Aplic formula descrisa in cerinta.
- cost_function.m: Aplic algoritmii descrisi in cerinta.
- predict_classes.m: Aplic algoritmul descris in cerinta.
