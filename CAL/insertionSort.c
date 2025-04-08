// v = [50,40,10,30];

void ordenaInsercao(int *v, int n) {
    int i, j, x; // O(1)

    for(i=0; i < n; i++) { // O(n)
        x = v[i]; // O(1)
        j = i - 1; // O(1)

        while(j >= 0 && v[j] > x) { // O (n - 1)
            v [j + 1] = v [j]; // O(1)
            j--; // O(1)
        }

        v[j + 1] = x; // O(1)
    }
}
// O(1) + O(n) * (O(1) + O (1) + O (n - 1)(O(1) + O(1) + O(1)))
// O(1) + O(n) * ( 2 * O(1) + O(n - 1) * (3 O(1)))
// O(1) + O(n) * O(n - 1)
// O(n^2)