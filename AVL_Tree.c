#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int key;
  int height;
  struct Node* left;
  struct Node* right;
} Node;

typedef struct AVLTree {
  Node* root;
} AVLTree;

int max(int a, int b) {
  return (a > b) ? a : b;
}

int getHeight(Node* node) {
  if (node == NULL) return 0;
  return node->height;
}

int getBalanceFactor(Node* node) {
  if (node == NULL) return 0;
  return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(Node* node) {
  if (node == NULL) return;
  node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

Node* rotateRight(Node* y) {
  Node* x = y->left;
  Node* z = x->right;
  x->right = y;
  y->left = z;
  updateHeight(y);
  updateHeight(x);
  return x;
}

Node* rotateLeft(Node* x) {
  Node* y = x->right;
  Node* z = y->left;
  y->left = x;
  x->right = z;
  updateHeight(x);
  updateHeight(y);
  return y;
}

Node* createNode(int key) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->key = key;
  newNode->height = 1;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

Node* insert(Node* node, int key) {
  if (node == NULL) return createNode(key);

  if (key < node->key) {
    node->left = insert(node->left, key);
  } else if (key > node->key) {
    node->right = insert(node->right, key);
  } else {
    return node;
  }

  updateHeight(node);
  int balance = getBalanceFactor(node);

  if (balance > 1 && key < node->left->key) {
    return rotateRight(node);
  }

  if (balance < -1 && key > node->right->key) {
    return rotateLeft(node);
  }

  if (balance > 1 && key > node->left->key) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  if (balance < -1 && key < node->right->key) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

Node* minValueNode(Node* node) {
  Node* current = node;
  while (current->left != NULL) {
    current = current->left;
  }
  return current;
}

Node* delete(Node* root, int key) {
  if (root == NULL) {
    return root;
  }

  if (key < root->key) {
    root->left = delete(root->left, key);
  } else if (key > root->key) {
    root->right = delete(root->right, key);
  } else {
    if ((root->left == NULL) || (root->right == NULL)) {
      Node* temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        *root = *temp;
      }
      free(temp);
    } else {
      Node* temp = minValueNode(root->right);
      root->key = temp->key;
      root->right = delete(root->right, temp->key);
    }
  }

  if (root == NULL) return root;

  updateHeight(root);
  int balance = getBalanceFactor(root);

  if (balance > 1 && getBalanceFactor(root->left) >= 0) {
    return rotateRight(root);
  }

  if (balance > 1 && getBalanceFactor(root->left) < 0) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }

  if (balance < -1 && getBalanceFactor(root->right) <= 0) {
    return rotateLeft(root);
  }

  if (balance < -1 && getBalanceFactor(root->right) > 0) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }

  return root;
}

Node* search(Node* node, int key) {
  if (node == NULL || node->key == key)
    return node;

  if (key < node->key)
    return search(node->left, key);

  return search(node->right, key);
}

void insertValue(AVLTree* tree, int value) {
  tree->root = insert(tree->root, value);
}

void deleteValue(AVLTree* tree, int value) {
  tree->root = delete(tree->root, value);
}

Node* searchValue(AVLTree* tree, int value) {
  return search(tree->root, value);
}

void preOrder(Node* root) {
  if (root != NULL) {
    printf("%d ", root->key);
    preOrder(root->left);
    preOrder(root->right);
  }
}

int main() {
  AVLTree tree;
  tree.root = NULL;

  insertValue(&tree, 10);
  insertValue(&tree, 20);
  insertValue(&tree, 30);
  insertValue(&tree, 40);
  insertValue(&tree, 50);
  insertValue(&tree, 25);

  printf("Preorder traversal of the constructed AVL tree is \n");
  preOrder(tree.root);

  deleteValue(&tree, 20);

  printf("\nPreorder traversal after deleting 20 \n");
  preOrder(tree.root);

  int valueToSearch = 30;
  Node* searchedNode = searchValue(&tree, valueToSearch);
  if (searchedNode != NULL) {
    printf("\nNode with key %d is present in the tree with height: %d\n", valueToSearch, searchedNode->height);
  } else {
    printf("\nNode with key %d is not present in the tree.\n", valueToSearch);
  }

  return 0;
}
