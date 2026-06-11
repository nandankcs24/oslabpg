#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INF 99999

// Shared Utility Functions
void swap(int* a, int* b) { int t = *a; *a = *b; *b = t; }
int max_val(int a, int b) { return (a > b) ? a : b; }

// ==============================================================================
// PROGRAM 1: Arrays, Stacks, Queues, Priority Queues (LC 268, 1636)
// ==============================================================================
int missingNumber(int* nums, int numsSize) {
    int expectedSum = numsSize * (numsSize + 1) / 2;
    int actualSum = 0;
    for(int i = 0; i < numsSize; i++) actualSum += nums[i];
    return expectedSum - actualSum;
}

typedef struct { int val; int freq; } Freq;

int cmpFreq(const void* a, const void* b) {
    Freq* f1 = (Freq*)a; Freq* f2 = (Freq*)b;
    if (f1->freq == f2->freq) return f2->val - f1->val;
    return f1->freq - f2->freq;
}

int* frequencySort(int* nums, int numsSize, int* returnSize) {
    int counts[201] = {0};
    for (int i = 0; i < numsSize; i++) counts[nums[i] + 100]++;
    Freq arr[numsSize];
    for (int i = 0; i < numsSize; i++) {
        arr[i].val = nums[i];
        arr[i].freq = counts[nums[i] + 100];
    }
    qsort(arr, numsSize, sizeof(Freq), cmpFreq);
    int* res = (int*)malloc(numsSize * sizeof(int));
    for (int i = 0; i < numsSize; i++) res[i] = arr[i].val;
    *returnSize = numsSize;
    return res;
}

int main_prog1() {
    int nums1[] = {3, 0, 1};
    printf("LC 268 (Missing Number): %d\n", missingNumber(nums1, 3));
    
    int nums2[] = {1, 1, 2, 2, 2, 3}, retSize;
    int* res = frequencySort(nums2, 6, &retSize);
    printf("LC 1636 (Frequency Sort): ");
    for(int i=0; i<retSize; i++) printf("%d ", res[i]);
    printf("\n");
    free(res);
    return 0;
}


// ==============================================================================
// PROGRAM 2: DFS, BFS Linked Lists & Arrays (LC 148, 215)
// ==============================================================================
struct ListNode { int val; struct ListNode *next; };

struct ListNode* mergeList(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy; dummy.next = NULL;
    struct ListNode* curr = &dummy;
    while(l1 && l2) {
        if(l1->val < l2->val) { curr->next = l1; l1 = l1->next; }
        else { curr->next = l2; l2 = l2->next; }
        curr = curr->next;
    }
    curr->next = l1 ? l1 : l2;
    return dummy.next;
}

struct ListNode* sortList(struct ListNode* head) {
    if(!head || !head->next) return head;
    struct ListNode *slow = head, *fast = head->next;
    while(fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    struct ListNode* mid = slow->next; slow->next = NULL;
    return mergeList(sortList(head), sortList(mid));
}

int cmpDesc(const void* a, const void* b) { return (*(int*)b - *(int*)a); }

int findKthLargest(int* nums, int numsSize, int k) {
    qsort(nums, numsSize, sizeof(int), cmpDesc);
    return nums[k - 1];
}

int main_prog2() {
    struct ListNode n1 = {4, NULL}, n2 = {2, NULL}, n3 = {1, NULL}, n4 = {3, NULL};
    n1.next = &n2; n2.next = &n3; n3.next = &n4;
    struct ListNode* sorted = sortList(&n1);
    printf("LC 148 (Sort List): ");
    while(sorted) { printf("%d ", sorted->val); sorted = sorted->next; }
    printf("\n");

    int nums[] = {3, 2, 1, 5, 6, 4};
    printf("LC 215 (Kth Largest): %d\n", findKthLargest(nums, 6, 2));
    return 0;
}


// ==============================================================================
// PROGRAM 3: Trees and Graphs (LC 455, 1791)
// ==============================================================================
int cmpInt(const void* a, const void* b) { return (*(int*)a - *(int*)b); }

int findContentChildren(int* g, int gSize, int* s, int sSize) {
    qsort(g, gSize, sizeof(int), cmpInt);
    qsort(s, sSize, sizeof(int), cmpInt);
    int i=0, j=0;
    while(i<gSize && j<sSize) {
        if(s[j] >= g[i]) i++;
        j++;
    }
    return i;
}

int findCenter(int** edges, int edgesSize, int* edgesColSize) {
    if(edges[0][0] == edges[1][0] || edges[0][0] == edges[1][1]) return edges[0][0];
    return edges[0][1];
}

int main_prog3() {
    int g[] = {1, 2, 3}, s[] = {1, 1};
    printf("LC 455 (Assign Cookies): %d\n", findContentChildren(g, 3, s, 2));

    int e1[] = {1, 2}, e2[] = {2, 3}, e3[] = {4, 2};
    int* edges[] = {e1, e2, e3};
    int colSize = 2;
    printf("LC 1791 (Find Center): %d\n", findCenter(edges, 3, &colSize));
    return 0;
}


// ==============================================================================
// PROGRAM 4: Topological Sort & LC 207
// ==============================================================================
void topologicalSort(int n, int adj[n][n]) {
    int indegree[100] = {0}, visited[100] = {0};
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            if(adj[i][j]) indegree[j]++;
    
    printf("Topological Sort: ");
    for(int count=0; count<n; count++) {
        for(int i=0; i<n; i++) {
            if(indegree[i] == 0 && !visited[i]) {
                printf("%d ", i);
                visited[i] = 1;
                for(int j=0; j<n; j++) if(adj[i][j]) indegree[j]--;
                break;
            }
        }
    }
    printf("\n");
}

int canFinish(int numCourses, int** prerequisites, int prerequisitesSize, int* prerequisitesColSize) {
    int* indegree = (int*)calloc(numCourses, sizeof(int));
    for(int i=0; i<prerequisitesSize; i++) indegree[prerequisites[i][0]]++;
    int* q = (int*)malloc(numCourses * sizeof(int));
    int front=0, rear=0, count=0;
    for(int i=0; i<numCourses; i++) if(indegree[i] == 0) q[rear++] = i;
    
    while(front < rear) {
        int curr = q[front++];
        count++;
        for(int i=0; i<prerequisitesSize; i++) {
            if(prerequisites[i][1] == curr) {
                indegree[prerequisites[i][0]]--;
                if(indegree[prerequisites[i][0]] == 0) q[rear++] = prerequisites[i][0];
            }
        }
    }
    free(indegree); free(q);
    return count == numCourses;
}

int main_prog4() {
    int n = 4;
    int adj[4][4] = {{0,1,0,0},{0,0,1,0},{0,0,0,1},{0,0,0,0}};
    topologicalSort(n, adj);

    int p1[] = {1, 0}, p2[] = {0, 1};
    int* pre[] = {p1, p2};
    int colSize = 2;
    printf("LC 207 (Course Schedule): %d\n", canFinish(2, pre, 2, &colSize));
    return 0;
}


// ==============================================================================
// PROGRAM 5: Johnson Trotter Algorithm (Permutations)
// ==============================================================================
int getMobile(int a[], int dir[], int n) {
    int mobile_prev = 0, mobile = 0;
    for (int i = 0; i < n; i++) {
        if ((dir[a[i]-1] == 0 && i != 0 && a[i] > a[i-1]) || (dir[a[i]-1] == 1 && i != n-1 && a[i] > a[i+1])) {
            if (a[i] > mobile_prev) {
                mobile = a[i];
                mobile_prev = mobile;
            }
        }
    }
    return mobile;
}

void johnsonTrotter(int n) {
    int a[100], dir[100];
    for (int i = 0; i < n; i++) { a[i] = i + 1; dir[i] = 0; printf("%d", a[i]); }
    printf("\n");
    int mobile = getMobile(a, dir, n);
    while (mobile != 0) {
        int pos;
        for (int i = 0; i < n; i++) if (a[i] == mobile) pos = i;
        if (dir[a[pos]-1] == 0) {
            int temp = a[pos]; a[pos] = a[pos-1]; a[pos-1] = temp;
        } else {
            int temp = a[pos]; a[pos] = a[pos+1]; a[pos+1] = temp;
        }
        for (int i = 0; i < n; i++) {
            if (a[i] > mobile) {
                if (dir[a[i]-1] == 0) dir[a[i]-1] = 1;
                else dir[a[i]-1] = 0;
            }
        }
        for (int i = 0; i < n; i++) printf("%d", a[i]);
        printf("\n");
        mobile = getMobile(a, dir, n);
    }
}

int main_prog5() {
    printf("Johnson Trotter Permutations for N=3:\n");
    johnsonTrotter(3);
    return 0;
}


// ==============================================================================
// PROGRAM 6: Merge Sort
// ==============================================================================
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for(int i=0; i<n1; i++) L[i] = arr[l + i];
    for(int j=0; j<n2; j++) R[j] = arr[m + 1 + j];
    int i=0, j=0, k=l;
    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main_prog6() {
    int n = 5000;
    int arr[5000];
    for(int i=0; i<n; i++) arr[i] = rand() % 10000;
    
    clock_t start = clock();
    mergeSort(arr, 0, n - 1);
    clock_t end = clock();
    
    printf("Merge Sort Time for %d elements: %f seconds\n", n, (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}


// ==============================================================================
// PROGRAM 7: Quick Sort
// ==============================================================================
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for(int j = low; j <= high - 1; j++) {
        if(arr[j] < pivot) { i++; swap(&arr[i], &arr[j]); }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main_prog7() {
    int n = 5000;
    int arr[5000];
    for(int i=0; i<n; i++) arr[i] = rand() % 10000;
    
    clock_t start = clock();
    quickSort(arr, 0, n - 1);
    clock_t end = clock();
    
    printf("Quick Sort Time for %d elements: %f seconds\n", n, (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}


// ==============================================================================
// PROGRAM 8: Heap Sort
// ==============================================================================
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if(l < n && arr[l] > arr[largest]) largest = l;
    if(r < n && arr[r] > arr[largest]) largest = r;
    if(largest != i) { swap(&arr[i], &arr[largest]); heapify(arr, n, largest); }
}

void heapSort(int arr[], int n) {
    for(int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for(int i = n - 1; i > 0; i--) { swap(&arr[0], &arr[i]); heapify(arr, i, 0); }
}

int main_prog8() {
    int n = 5000;
    int arr[5000];
    for(int i=0; i<n; i++) arr[i] = rand() % 10000;
    
    clock_t start = clock();
    heapSort(arr, n);
    clock_t end = clock();
    
    printf("Heap Sort Time for %d elements: %f seconds\n", n, (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}


// ==============================================================================
// PROGRAM 9: 0/1 Knapsack & LC 509 (Fibonacci)
// ==============================================================================
int knapSack(int W, int wt[], int val[], int n) {
    int K[n + 1][W + 1];
    for(int i = 0; i <= n; i++) {
        for(int w = 0; w <= W; w++) {
            if(i == 0 || w == 0) K[i][w] = 0;
            else if(wt[i - 1] <= w) K[i][w] = max_val(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else K[i][w] = K[i - 1][w];
        }
    }
    return K[n][W];
}

int fib(int n) {
    if(n <= 1) return n;
    int a = 0, b = 1, c;
    for(int i=2; i<=n; i++) { c = a + b; a = b; b = c; }
    return b;
}

int main_prog9() {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    printf("Max 0/1 Knapsack Value: %d\n", knapSack(50, wt, val, 3));
    
    printf("LC 509 (Fibonacci 10): %d\n", fib(10));
    return 0;
}


// ==============================================================================
// PROGRAM 10: Floyd's Algorithm & LC 338 (Counting Bits)
// ==============================================================================
void floydWarshall(int n, int graph[][n]) {
    int dist[n][n];
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++) dist[i][j] = graph[i][j];
    for(int k=0; k<n; k++) {
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                if(dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    printf("Floyd Shortest Paths:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(dist[i][j] == INF) printf("INF ");
            else printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
}

int* countBits(int n, int* returnSize) {
    int* ans = (int*)malloc((n + 1) * sizeof(int));
    ans[0] = 0;
    for(int i=1; i<=n; i++) ans[i] = ans[i >> 1] + (i & 1);
    *returnSize = n + 1;
    return ans;
}

int main_prog10() {
    int graph[4][4] = {{0, 5, INF, 10}, {INF, 0, 3, INF}, {INF, INF, 0, 1}, {INF, INF, INF, 0}};
    floydWarshall(4, graph);
    
    int rs;
    int* bits = countBits(5, &rs);
    printf("LC 338 (Counting Bits to 5): ");
    for(int i=0; i<rs; i++) printf("%d ", bits[i]);
    printf("\n");
    free(bits);
    return 0;
}


// ==============================================================================
// PROGRAM 11: Prim's and Kruskal's Algorithms
// ==============================================================================
void primMST(int n, int graph[n][n]) {
    int parent[n], key[n], mstSet[n];
    for(int i=0; i<n; i++) { key[i] = INF; mstSet[i] = 0; }
    key[0] = 0; parent[0] = -1;
    for(int count=0; count<n-1; count++) {
        int min = INF, u = -1;
        for(int v=0; v<n; v++) if(mstSet[v] == 0 && key[v] < min) { min = key[v]; u = v; }
        if (u != -1) {
            mstSet[u] = 1;
            for(int v=0; v<n; v++) {
                if(graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) {
                    parent[v] = u; key[v] = graph[u][v];
                }
            }
        }
    }
    int cost = 0;
    for(int i=1; i<n; i++) cost += graph[i][parent[i]];
    printf("Prim MST Cost: %d\n", cost);
}

int kruskal_parent[100];
int find(int i) { while(kruskal_parent[i]) i = kruskal_parent[i]; return i; }
int uni(int i, int j) {
    if(i != j) { kruskal_parent[j] = i; return 1; }
    return 0;
}

void kruskalMST(int n, int cost[n][n]) {
    int mincost = 0, ne = 1;
    for(int i=0; i<n; i++) kruskal_parent[i] = 0;
    while(ne < n) {
        int min = INF, a = -1, b = -1, u = -1, v = -1;
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                if(cost[i][j] < min) { min = cost[i][j]; a = u = i; b = v = j; }
            }
        }
        u = find(u); v = find(v);
        if(uni(u, v)) { mincost += min; ne++; }
        cost[a][b] = cost[b][a] = INF;
    }
    printf("Kruskal MST Cost: %d\n", mincost);
}

int main_prog11() {
    int graph[5][5] = { {0,2,0,6,0}, {2,0,3,8,5}, {0,3,0,0,7}, {6,8,0,0,9}, {0,5,7,9,0} };
    primMST(5, graph);
    
    int cost[5][5];
    for(int i=0; i<5; i++) 
        for(int j=0; j<5; j++) 
            cost[i][j] = graph[i][j] ? graph[i][j] : INF;
            
    kruskalMST(5, cost);
    return 0;
}


// ==============================================================================
// PROGRAM 12: Fractional Knapsack (Greedy)
// ==============================================================================
void fractionalKnapsack(int n, float weight[], float profit[], float capacity) {
    float ratio[100], temp;
    for(int i=0; i<n; i++) ratio[i] = profit[i] / weight[i];
    for(int i=0; i<n-1; i++) {
        for(int j=i+1; j<n; j++) {
            if(ratio[i] < ratio[j]) {
                temp = ratio[j]; ratio[j] = ratio[i]; ratio[i] = temp;
                temp = weight[j]; weight[j] = weight[i]; weight[i] = temp;
                temp = profit[j]; profit[j] = profit[i]; profit[i] = temp;
            }
        }
    }
    float tp = 0;
    int i;
    for(i=0; i<n; i++) {
        if(weight[i] > capacity) break;
        else { tp = tp + profit[i]; capacity = capacity - weight[i]; }
    }
    if(i < n) tp = tp + (ratio[i] * capacity);
    printf("Max Fractional Profit: %f\n", tp);
}

int main_prog12() {
    float weight[] = {10, 20, 30};
    float profit[] = {60, 100, 120};
    fractionalKnapsack(3, weight, profit, 50);
    return 0;
}


// ==============================================================================
// PROGRAM 13: Dijkstra's Algorithm
// ==============================================================================
void dijkstra(int n, int graph[n][n], int startnode) {
    int cost[100][100], distance[100], visited[100];
    int count, mindistance, nextnode;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++) cost[i][j] = graph[i][j] == 0 ? INF : graph[i][j];
    for(int i=0; i<n; i++) { distance[i] = cost[startnode][i]; visited[i] = 0; }
    distance[startnode] = 0; visited[startnode] = 1; count = 1;
    
    while(count < n - 1) {
        mindistance = INF;
        for(int i=0; i<n; i++) {
            if(distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i]; nextnode = i;
            }
        }
        visited[nextnode] = 1;
        for(int i=0; i<n; i++) {
            if(!visited[i] && mindistance + cost[nextnode][i] < distance[i])
                distance[i] = mindistance + cost[nextnode][i];
        }
        count++;
    }
    for(int i=0; i<n; i++) 
        if(i != startnode) 
            printf("Distance to %d: %d\n", i, distance[i]);
}

int main_prog13() {
    int graph[5][5] = {{0,10,0,30,100}, {10,0,50,0,0}, {0,50,0,20,10}, {30,0,20,0,60}, {100,0,10,60,0}};
    dijkstra(5, graph, 0);
    return 0;
}


// ==============================================================================
// PROGRAM 14: N-Queens Problem (Backtracking)
// ==============================================================================
int board[20], q_count = 0;

int place(int row, int column) {
    for(int i=1; i<=row-1; ++i) {
        if(board[i] == column || abs(board[i] - column) == abs(i - row)) return 0;
    }
    return 1;
}

void nqueens(int row, int n) {
    for(int column=1; column<=n; ++column) {
        if(place(row, column)) {
            board[row] = column;
            if(row == n) q_count++;
            else nqueens(row+1, n);
        }
    }
}

int main_prog14() {
    int n = 8;
    nqueens(1, n);
    printf("Solutions for %d-Queens: %d\n", n, q_count);
    return 0;
}