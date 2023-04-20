#include <iostream>
#include <vector>
#include <stack>
using namespace std;

const int order = 5;

// Assuming key and value are the types of the keys and values in the tree
// and order is the maximum number of keys in a node

// Node is a struct that represents a node in the B+ tree
struct Node {
    bool is_leaf; // whether the node is a leaf node or not
    int num_keys; // the number of keys in the node
    int keys[order]; // the array of keys in the node
    Node* pointers[order + 1]; // the array of pointers to child nodes or values
    Node* next; // the pointer to the next leaf node (only used for leaf nodes)

    // Node constructor
    Node(bool is_leaf) {
        this->is_leaf = is_leaf;
        this->num_keys = 0;
        this->next = nullptr;
    }
};

Node* root;

// find(v) returns the value associated with the key v in the tree
// or null if v is not found
Node* find(int v) {
    Node* C = root; // C is the current node
    while (!C->is_leaf) { // while C is not a leaf node
        int i = 0; // i is the index of the key or pointer to follow
        while (i < C->num_keys && v > C->keys[i]) { // find the least number i s.t. v <= Ki
            i++;
        }
        if (i == C->num_keys) { // if there is no such number i
            C = C->pointers[C->num_keys]; // set C to the last non-null pointer in C
        }
        else if (v == C->keys[i]) { // else if v = Ki
            C = C->pointers[i + 1]; // set C to Pi + 1
        }
        else { // else v < Ki
            C = C->pointers[i]; // set C to Pi
        }
    }
    // C is a leaf node now
    for (int i = 0; i < C->num_keys; i++) { // loop through the keys in C
        if (C->keys[i] == v) { // if Ki = v
            return C->pointers[i]; // return Pi (the value associated with v)
        }
    }
    return nullptr; // v is not found in the tree
}

// insert_in_leaf(L, K, P) inserts the value K and the pointer P into the leaf node L
void insert_in_leaf(Node* L, int K, Node* P) {
    if (K < L->keys[0]) { // if K < L.K1
      // insert (P, K) into L just before L.P1
        for (int i = L->num_keys; i > 0; i--) { // shift the keys and pointers to the right
            L->keys[i] = L->keys[i - 1];
            L->pointers[i] = L->pointers[i - 1];
        }
        L->keys[0] = K; // insert K at the first position
        L->pointers[0] = P; // insert P at the first position
        L->num_keys++; // increment the number of keys in L
    }
    else { // else
   // find the highest value Ki in L that is less than or equal to K
        int i = 0;
        while (i < L->num_keys&& K >= L->keys[i]) {
            i++;
        }
        // insert (P, K) into L just after Ki
        for (int j = L->num_keys; j > i; j--) { // shift the keys and pointers to the right
            L->keys[j] = L->keys[j - 1];
            L->pointers[j] = L->pointers[j - 1];
        }
        L->keys[i] = K; // insert K at the i-th position
        L->pointers[i] = P; // insert P at the i-th position
        L->num_keys++; // increment the number of keys in L
    }
}

// parent(N) returns the parent node of N in the tree
Node* parent(Node* N) {
    if (N == root) { // if N is the root of the tree
        return nullptr; // return null as there is no parent
    }
    Node* C = root; // C is the current node
    while (!C->is_leaf) { // while C is not a leaf node
        for (int i = 0; i <= C->num_keys; i++) { // loop through the pointers in C
            if (C->pointers[i] == N) { // if Pi = N
                return C; // return C as the parent of N
            }
        }
        // find the child node of C that contains N
        int i = 0; // i is the index of the key or pointer to follow
        while (i < C->num_keys && N->keys[0] > C->keys[i]) { // find the least number i s.t. N.K1 <= Ci
            i++;
        }
        C = C->pointers[i]; // set C to Pi (the child node that contains N)
    }
    return nullptr; // return null if no parent is found (should not happen)
}

// insert_in_parent(N, K1, N1) inserts the value K1 and the pointer N1 into the parent of N
void insert_in_parent(Node* N, int K1, Node* N1) {
    if (N == root) { // if N is the root of the tree
      // create a new node R containing (N, K1, N1)
        Node* R = new Node(false); // R is a non-leaf node
        R->keys[0] = K1; // R.K1 = K1
            R->pointers[0] = N; // R.P1 = N
        R->pointers[1] = N1; // R.P2 = N1
            R->num_keys = 1; // R has one key
            // make R the root of the tree
        root = R;
        return;
    }
    // else
    Node* P = parent(N); // P is the parent of N
    if (P->num_keys < order) { // if P has less than n pointers
      // insert (K1, N1) in P just after N
        int i = 0; // i is the index of N in P
        while (P->pointers[i] != N) { // find the index of N in P
            i++;
        }
        for (int j = P->num_keys; j > i; j--) { // shift the keys and pointers to the right
            P->keys[j] = P->keys[j - 1];
            P->pointers[j + 1] = P->pointers[j];
        }
        P->keys[i] = K1; // insert K1 at the i - th position
            P->pointers[i + 1] = N1; // insert N1 at the i + 1 - th position
            P->num_keys++; // increment the number of keys in P
    }
    else { // else split P
         // copy P to a block of memory T that can hold P and (K1, N1)
        Node* T = new Node(false); // T is a non-leaf node
        for (int k = 0; k < P->num_keys; k++) { // copy the keys and pointers from P to T
            T->keys[k] = P->keys[k];
            T->pointers[k] = P->pointers[k];
        }
        T->pointers[P->num_keys] = P->pointers[P->num_keys]; // copy the last pointer from P to T
        T->num_keys = P->num_keys; // copy the number of keys from P to T
        // insert (K1, N1) into T just after N
        int i = 0; // i is the index of N in T
        while (T->pointers[i] != N) { // find the index of N in T
            i++;
        }
        for (int j = T->num_keys; j > i; j--) { // shift the keys and pointers to the right
            T->keys[j] = T->keys[j - 1];
            T->pointers[j + 1] = T->pointers[j];
        }
        T->keys[i] = K1; // insert K1 at the i - th position
            T->pointers[i + 1] = N1; // insert N1 at the i + 1 - th position
            T->num_keys++; // increment the number of keys in T
         // erase all entries from P
        for (int k = 0; k < P->num_keys; k++) { // erase the keys and pointers from P
            P->keys[k] = 0;
            P->pointers[k] = nullptr;
        }
        P->pointers[P->num_keys] = nullptr; // erase the last pointer from P
        P->num_keys = 0; // set the number of keys in P to zero
        // create node P1
        Node* P1 = new Node(false); // P1 is a non - leaf node
            // copy T.P1...T.P⌈(n+1)/2⌉ into P
            int m = (order + 1) / 2; // m is the middle index of T
        for (int k = 0; k < m; k++) { // copy the first m keys and pointers from T to P
            P->keys[k] = T->keys[k];
            P->pointers[k] = T->pointers[k];
        }
        P->pointers[m] = T->pointers[m]; // copy the m-th pointer from T to P
        P->num_keys = m; // set the number of keys in P to m
        // let K2 = T.K⌈(n+1)/2⌉
        int K2 = T->keys[m]; // K2 is the middle key of T
         // copy T.P⌈(n+1)/2⌉+1...T.Pn+1 into P1
        for (int k = m + 1; k <= order; k++) { // copy the last order - m keys and pointers from T to P1
            P1->keys[k - m - 1] = T->keys[k];
                P1->pointers[k - m - 1] = T->pointers[k];
        }
        P1->num_keys = order - m; // set the number of keys in P1 to order - m
            // insert_in_parent(P, K2, P1)
            insert_in_parent(P, K2, P1);
    }
}

// insert(K, P) inserts the value K and the pointer P into the tree
void insert(int K, Node* P) {
    if (root == nullptr) { // if tree is empty
      // create an empty leaf node L, which is also the root
        Node* L = new Node(true); // L is a leaf node
        root = L;
    }
    else { // else
   // find(K) /* Find the leaf node L that should contain key value K */
        Node* L = find(K); // L is the leaf node that should contain K
        if (L->num_keys < order - 1) { // if L has less than n - 1 key values
          // insert_in_leaf(L, K, P)
            insert_in_leaf(L, K, P);
        }
        else { // else L has n - 1 key values already, split it
            // create node L1
            Node * L1 = new Node(true); // L1 is a leaf node
                // copy L.P1 ... L.Kn-1 to a block of memory T that can hold n (pointer, key-value) pairs
                Node * T = new Node(true); // T is a leaf node
            for (int i = 0; i < order - 1; i++) { // copy the keys and pointers from L to T
                T->keys[i] = L->keys[i];
                T->pointers[i] = L->pointers[i];
            }
            T->num_keys = order - 1; // copy the number of keys from L to T
            // insert_in_leaf(T, K, P)
            insert_in_leaf(T, K, P);
            // set L1.Pn = L.Pn
            L1->next = L->next; // L1.Pn is the pointer to the next leaf node
                // set L.Pn = L1
                L->next = L1; // L.Pn is the pointer to the next leaf node
                // erase L.P1 through L.Kn-1 from L
                for (int i = 0; i < order - 1; i++) { // erase the keys and pointers from L
                    L->keys[i] = 0;
                    L->pointers[i] = nullptr;
                }
            L->num_keys = 0; // set the number of keys in L to zero
            // copy T.P1 through T.K⌈n/2⌉ from T into L starting at L.P1
            int m = order / 2; // m is the middle index of T
            for (int i = 0; i < m; i++) { // copy the first m keys and pointers from T to L
                L->keys[i] = T->keys[i];
                L->pointers[i] = T->pointers[i];
            }
            L->num_keys = m; // set the number of keys in L to m
            // copy T.P⌈n/2⌉+1 through T.Kn from T into L1 starting at L1.P1
            for (int i = m; i <= order; i++) { // copy the last order - m + 1 keys and pointers from T to L1
                L1->keys[i - m] = T->keys[i];
                    L1->pointers[i - m] = T->pointers[i];
            }
            L1->num_keys = order - m + 1; // set the number of keys in L1 to order - m + 1
                // let K1 be the smallest key-value in L1
                int K1 = L1->keys[0]; // K1 is the first key in L1
                // insert_in_parent(L, K1, L1)
            insert_in_parent(L, K1, L1);
        }
    }
}

int main() {
	return 0;
}