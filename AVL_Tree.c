#include <stdio.h>
#include <stdlib.h>

// 노드 구조체 정의
typedef struct Node {
  int key;
  int height;
  struct Node* left;
  struct Node* right;
} Node;

// AVL 트리 구조체 정의
typedef struct AVLTree {
  Node* root;
} AVLTree;

// 두 정수 중 더 큰 값을 반환하는 함수
int max(int a, int b) {
  return (a > b) ? a : b;
}

// 노드의 높이를 반환하는 함수
int getHeight(Node* node) {
  if (node == NULL) return 0;
  return node->height;
}


// 노드의 균형 인자를 반환하는 함수
int getBalanceFactor(Node* node) {
  if (node == NULL) return 0;
  return getHeight(node->left) - getHeight(node->right);
}


// 노드의 높이를 업데이트하는 함수
void updateHeight(Node* node) {
  if (node == NULL) return;
  node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

// 우회전 함수
Node* rotateRight(Node* y) {
  Node* x = y->left;
  Node* z = x->right;

  // 회전 작업
  x->right = y;
  y->left = z;
  
  // 높이 업데이트
  updateHeight(y);
  updateHeight(x);
  
  return x;
}

// 좌회전 함수
Node* rotateLeft(Node* x) {
  Node* y = x->right;
  Node* z = y->left;
  
  // 회전 작업
  y->left = x;
  x->right = z;
  
  // 높이 업데이트
  updateHeight(x);
  updateHeight(y);
  
  return y;
}

// 새 노드를 생성하고 초기화하는 함수
Node* createNode(int key) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  newNode->key = key;
  newNode->height = 1;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

// 노드를 삽입하는 함수 (재귀적 호출)
Node* insert(Node* node, int key) {
  // 기본 삽입 작업
  if (node == NULL) return createNode(key);

  if (key < node->key) {
    node->left = insert(node->left, key);
  } else if (key > node->key) {
    node->right = insert(node->right, key);
  } else {
    return node;
  }

  // 높이 업데이트
  updateHeight(node);
  
  // 균형 인자 계산
  int balance = getBalanceFactor(node);

  // 균형을 맞추는 회전 작업
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

// 최소값을 가진 노드를 찾아 반환하는 함수
Node* minValueNode(Node* node) {
  Node* current = node;
  while (current->left != NULL) {
    current = current->left;
  }
  return current;
}

// 노드를 삭제하는 함수 (재귀적 호출)
Node* delete(Node* root, int key) {
  // 기본 삭제 작업
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

  // 균형 유지 작업
  if (root == NULL) return root;

  // 높이 업데이트
  updateHeight(root);
  
  // 균형 인자 계산
  int balance = getBalanceFactor(root);

  // 균형을 맞추는 회전 작업
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

// 노드를 탐색하는 함수
Node* search(Node* node, int key) {
  if (node == NULL || node->key == key)
    return node;

  if (key < node->key)
    return search(node->left, key);

  return search(node->right, key);
}

// 트리에 값을 삽입하는 함수
void insertValue(AVLTree* tree, int value) {
  tree->root = insert(tree->root, value);
}

// 트리에서 값을 삭제하는 함수
void deleteValue(AVLTree* tree, int value) {
  tree->root = delete(tree->root, value);
}

// 트리에서 값을 찾는 함수
Node* searchValue(AVLTree* tree, int value) {
  return search(tree->root, value);
}

// 전위 순회 함수
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

  // 삽입 예제
  insertValue(&tree, 10);
  insertValue(&tree, 20);
  insertValue(&tree, 30);
  insertValue(&tree, 40);
  insertValue(&tree, 50);
  insertValue(&tree, 25);

  // 출력 예제
  printf("생성된 AVL tree : \n");
  preOrder(tree.root);
  printf("\n");
  
  // 삭제 예제
  deleteValue(&tree, 20);
  
  printf("\n20을 삭제한 후의 값 : \n");
  preOrder(tree.root);
  printf("\n");
  
  // 탐색 예제
  int valueToSearch = 30;
  Node* searchedNode = searchValue(&tree, valueToSearch);
  if (searchedNode != NULL) {
    printf("\n노드 값 %d 이 위치하고 있는 트리의 height는 %d 입니다.\n", valueToSearch, searchedNode->height);
  } else {
    printf("\n노드 값 %d 은 해당 트리에 존재하지 않습니다.\n", valueToSearch);
  }

  return 0;
}
