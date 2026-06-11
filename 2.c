#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ==============================================================================
// 1. Russian Peasant Multiplication
// ==============================================================================
// Multiplies two numbers by halving the first and doubling the second.
int russianPeasant(int a, int b) {
    int res = 0;
    while (b > 0) {
        if (b & 1) res += a; // If b is odd, add 'a' to result
        a <<= 1;             // Double a
        b >>= 1;             // Halve b
    }
    return res;
}

// ==============================================================================
// 2. Warshall's Algorithm
// ==============================================================================
// Computes the transitive closure of a directed graph.
void warshall(int n, int graph[n][n]) {
    int tc[n][n];
    for(int i = 0; i < n; i++) 
        for(int j = 0; j < n; j++) 
            tc[i][j] = graph[i][j];

    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                tc[i][j] = tc[i][j] || (tc[i][k] && tc[k][j]);
            }
        }
    }

    printf("Warshall's Transitive Closure:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) printf("%d ", tc[i][j]);
        printf("\n");
    }
}

// ==============================================================================
// 3. Computing Median and the Selection Problem (Quickselect)
// ==============================================================================
// Finds the k-th smallest element in an unordered array.
void swap(int* a, int* b) { int t = *a; *a = *b; *b = t; }

int partition(int arr[], int l, int r) {
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++) {
        if (arr[j] <= x) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}

int kthSmallest(int arr[], int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int pos = partition(arr, l, r);
        if (pos - l == k - 1) return arr[pos];
        if (pos - l > k - 1) return kthSmallest(arr, l, pos - 1, k);
        return kthSmallest(arr, pos + 1, r, k - pos + l - 1);
    }
    return INT_MAX;
}

// ==============================================================================
// 4. Coin Problem (Minimum Coin Change using DP)
// ==============================================================================
// Finds the minimum number of coins to make a given value.
int minCoins(int coins[], int m, int V) {
    int table[V + 1];
    table[0] = 0;
    for(int i = 1; i <= V; i++) table[i] = INT_MAX;

    for(int i = 1; i <= V; i++) {
        for(int j = 0; j < m; j++) {
            if(coins[j] <= i) {
                int sub_res = table[i - coins[j]];
                if(sub_res != INT_MAX && sub_res + 1 < table[i])
                    table[i] = sub_res + 1;
            }
        }
    }
    return table[V] == INT_MAX ? -1 : table[V];
}

// ==============================================================================
// 5. Subset-Sum Problem (using DP)
// ==============================================================================
// Determines if there is a subset of the given set with sum equal to given sum.
int isSubsetSum(int set[], int n, int sum) {
    int subset[n + 1][sum + 1];
    for (int i = 0; i <= n; i++) subset[i][0] = 1; // Sum 0 is always possible
    for (int i = 1; i <= sum; i++) subset[0][i] = 0; // 0 elements can't make >0 sum

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (j < set[i - 1]) 
                subset[i][j] = subset[i - 1][j];
            else 
                subset[i][j] = subset[i - 1][j] || subset[i - 1][j - set[i - 1]];
        }
    }
    return subset[n][sum];
}

// ==============================================================================
// Main Execution block
// ==============================================================================
int main() {
    // 1. Russian Peasant
    printf("1. Russian Peasant Multiplication: 18 * 23 = %d\n\n", russianPeasant(18, 23));

    // 2. Warshall
    int graph[4][4] = { {0, 1, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 0}, {1, 0, 1, 0} };
    printf("2. ");
    warshall(4, graph);
    printf("\n");

    // 3. Median / Selection
    int arr[] = {12, 3, 5, 7, 4, 19, 26};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = n / 2 + 1; // Median element position (1-based index)
    printf("3. Selection Problem: The %d-th smallest element (Median) is %d\n\n", k, kthSmallest(arr, 0, n - 1, k));

    // 4. Coin Problem
    int coins[] = {9, 6, 5, 1};
    int m = sizeof(coins) / sizeof(coins[0]);
    int V = 11;
    printf("4. Coin Problem: Minimum coins required for value %d is %d\n\n", V, minCoins(coins, m, V));

    // 5. Subset-Sum Problem
    int set[] = {3, 34, 4, 12, 5, 2};
    int sum = 9;
    int set_n = sizeof(set) / sizeof(set[0]);
    if (isSubsetSum(set, set_n, sum))
        printf("5. Subset-Sum: Found a subset with given sum %d\n", sum);
    else
        printf("5. Subset-Sum: No subset with given sum %d\n", sum);

    return 0;
}