
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"
#include "Record.h"
#include "AVLTree.h"

#define new_max(x,y) (((x) >= (y)) ? (x) : (y))

typedef struct node *Node;
struct node {
    Record rec;
    Node   left;
    Node   right;
    int    height;
};

struct tree {
    Node    root;
    int     (*compare)(Record, Record);
};

////////////////////////////////////////////////////////////////////////
// Auxiliary functions

static void doTreeFree(Node n, bool freeRecords);
static Node newNode(Record rec);
static Record doTreeSearch(Tree t, Node n, Record rec);


////////////////////////////////////////////////////////////////////////

static Node newNode(Record rec) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    n->rec = rec;
    n->left = NULL;
    n->right = NULL;
    n->height = 0;
    return n;
}

////////////////////////////////////////////////////////////////////////

Tree TreeNew(int (*compare)(Record, Record)) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    t->root = NULL;
    t->compare = compare;
    return t;
}

////////////////////////////////////////////////////////////////////////

void TreeFree(Tree t, bool freeRecords) {
    doTreeFree(t->root, freeRecords);
    free(t);
}

static void doTreeFree(Node n, bool freeRecords) {
    if (n != NULL) {
        doTreeFree(n->left, freeRecords);
        doTreeFree(n->right, freeRecords);
        if (freeRecords) {
            RecordFree(n->rec);
        }
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////

Record TreeSearch(Tree t, Record rec) {
    return doTreeSearch(t, t->root, rec);
}

static Record doTreeSearch(Tree t, Node n, Record rec) {
    if (n == NULL) {
        return NULL;
    }

    int cmp = t->compare(rec, n->rec);
    if (cmp < 0) {
        return doTreeSearch(t, n->left, rec);
    } else if (cmp > 0) {
        return doTreeSearch(t, n->right, rec);
    } else {
        return n->rec;
    }
}


////////////////////////////////////////////////////////////////////////
/* IMPORTANT: 
   Do NOT modify the code above this line. 
   You must not modify the 'node' and 'tree' structures defined above.
   You must not modify the functions defined above.
*/
////////////////////////////////////////////////////////////////////////
// Self made functions
static Node doTreeInsert(Tree t, Node n, Record rec, bool *res);
static void doTreeSearchBetween(Tree t, Node n, Record lower, Record upper, List L);
static Record doTreeNext(Tree t, Node n, Record r, Record currlargest);
static Node rotateRight(Node n1);
static Node rotateLeft(Node n2);
////////////////////////////////////////////////////////////////////////

// Used lecture code as well as code from lab04 to help create this function
bool TreeInsert(Tree t, Record rec) {
    // res will remain false until Record rec is inserted
    bool res = false;
    t->root =  doTreeInsert(t, t->root, rec, &res);
    return res;
}

static Node doTreeInsert(Tree t, Node n, Record rec, bool *res) {

    // Record Insertion

    // Inserting the record into AVL tree
    if (n == NULL) {
        *res = true;
        return newNode(rec);
    }
    
    // Comparing root node with record and inserting in correct spot 
    int cmp_node = t->compare(rec, n->rec);
    // If a record already exists with the given rec
    if (cmp_node == 0) {
        *res = false;
    }
    // Searching left node
    else if (cmp_node < 0) {
        n->left = doTreeInsert(t, n->left, rec, res);
    }
    // Searching right node
    else if (cmp_node > 0) {
        n->right = doTreeInsert(t, n->right, rec, res);
    }

    // Balancing AVLTree

    // Calculating height to ensure AVL tree is balanced 
    int leftHeight = 0;
    if (n->left != NULL) {
        leftHeight = n->left->height;
    }
    int rightHeight = 0;
    if (n->right != NULL) {
        rightHeight = n->right->height;
    }
    int height_diff = leftHeight - rightHeight;
    // If difference between left and right height is greater than 1
    if (height_diff > 1) {
        // If Right left rotation is required
        if (t->compare(rec, n->left->rec) > 0) {
            n->left = rotateLeft(n->left);
        }
        n = rotateRight(n);
    }
    // If difference between right and left height is greater than 1
    else if (height_diff < -1) {
        // If Left right rotation is required
        if (t->compare(rec, n->right->rec) < 0) {
            n->right = rotateRight(n->right);
        }
        n = rotateLeft(n);
    }
    // Assigning heights to node n
    leftHeight = 0;
    rightHeight = 0;
    if (n->left != NULL) {
        leftHeight = n->left->height;
    }
    if (n->right != NULL) {
        rightHeight = n->right->height;
    }
    n->height = new_max(leftHeight, rightHeight) + 1;

    return n;
}

////////////////////////////////////////////////////////////////////////
List TreeSearchBetween(Tree t, Record lower, Record upper) {
    List L = ListNew();
    Node n = t->root;
    doTreeSearchBetween(t, n, lower, upper, L);
    return L;
}

static void doTreeSearchBetween(Tree t, Node n, Record lower, Record upper, List L) {
    if (n == NULL) {
        return;
    }
    int cmpLow = t->compare(n->rec, lower);

    int cmpUp = t->compare(n->rec, upper);
    // If rec is greater than lower then search right node
    if (cmpLow < 0) {
        doTreeSearchBetween(t, n->right, lower, upper, L);
    } 
    // If rec is less than up search left node
    else if (cmpUp > 0) {
        doTreeSearchBetween(t, n->left, lower, upper, L);
    }
    // If rec is inbetween the lower and upper
    else {
        //Appending to the list in ascending order
        doTreeSearchBetween(t, n->left, lower, upper, L);
        ListAppend(L, n->rec);
        doTreeSearchBetween(t, n->right, lower, upper, L);
    }

}

////////////////////////////////////////////////////////////////////////

Record TreeNext(Tree t, Record r) {
    Record currlargest = NULL;
    return doTreeNext(t, t->root, r, currlargest);
}

static Record doTreeNext(Tree t, Node n, Record r, Record currlargest) {
    if (t->root == NULL) {
        return NULL;
    }
    // Will return NULL if no larger record has been found otherwise will return the next largest record found
    if (n == NULL) {
        return currlargest;
    }
    // A record that is greater than the given record has been found however not sure that it is
    // the smallest record greater than the given record
    if (t->compare(n->rec, r) > 0) {
        currlargest = n->rec;
        return doTreeNext(t, n->left, r, currlargest);
    } 
    // Record is less than given record
    else if (t->compare(n->rec, r) < 0) {
        return doTreeNext(t, n->right, r, currlargest);
    }
    return r;

}

// Help of lecture code for both Rotate functions
static Node rotateRight(Node n1) {
    if (n1 == NULL || n1->left == NULL) {
        return n1;
    }
    Node n2 = n1->left;
    n1->left = n2->right;
    n2->right = n1;
    // Height update
    n1->height--;
    n2->height++;
    return n2;
}

static Node rotateLeft(Node n2) {
    if (n2 == NULL || n2->right == NULL) {
        return n2;
    }
    Node n1 = n2->right;
    n2->right = n1->left;
    n1->left = n2;
    // Height update
    n2->height--;
    n1->height++;
    return n1;
}