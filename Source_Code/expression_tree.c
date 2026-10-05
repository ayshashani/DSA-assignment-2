#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Structure for expression tree node
struct Node {
    char data;
    struct Node *left;
    struct Node *right;
};


struct Node *treeStack[MAX];
int top = -1;


int valueStack[MAX];
int vtop = -1;


struct Node* createNode(char data) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


void pushTree(struct Node *node) {
    treeStack[++top] = node;
}


struct Node* popTree() {
    return treeStack[top--];
}


int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '^');
}


struct Node* buildExpressionTree(char postfix[]) {
    int i;
    char ch;

    for (i = 0; postfix[i] != '\0'; i++) {
        ch = postfix[i];

        
        if (ch == ' ')
            continue;

        
        if (isdigit(ch)) {
            struct Node *node = createNode(ch);
            pushTree(node);
        }

        
        else if (isOperator(ch)) {
            struct Node *node = createNode(ch);

            node->right = popTree();
            node->left = popTree();

            pushTree(node);
        }
    }

    return popTree();
}


void inorder(struct Node *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%c ", root->data);
        inorder(root->right);
    }
}


void preorder(struct Node *root) {
    if (root != NULL) {
        printf("%c ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct Node *root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%c ", root->data);
    }
}


int calculate(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}


int evaluateTree(struct Node *root) {
    int leftValue, rightValue;

    if (root == NULL)
        return 0;

  
    if (!isOperator(root->data))
        return root->data - '0';

    leftValue = evaluateTree(root->left);
    rightValue = evaluateTree(root->right);

    return calculate(leftValue, rightValue, root->data);
}


int evaluatePostfix(char postfix[]) {
    int i;
    char ch;

    vtop = -1;

    for (i = 0; postfix[i] != '\0'; i++) {
        ch = postfix[i];

        if (ch == ' ')
            continue;

        
        if (isdigit(ch)) {
            valueStack[++vtop] = ch - '0';
        }

        // If operator, pop two values
        else if (isOperator(ch)) {
            int b = valueStack[vtop--];
            int a = valueStack[vtop--];

            valueStack[++vtop] = calculate(a, b, ch);
        }
    }

    return valueStack[vtop];
}

int main() {
    char postfix[] = "8 3 2 * + 6 2 / -";

    struct Node *root;

    printf("Postfix Expression: %s\n\n", postfix);

    
    root = buildExpressionTree(postfix);

    printf("Expression Tree Traversals:\n");

    printf("Inorder   : ");
    inorder(root);

    printf("\nPreorder  : ");
    preorder(root);

    printf("\nPostorder : ");
    postorder(root);

    
    printf("\n\nEvaluation Results:\n");

    printf("Postfix Evaluation using Stack = %d\n",
           evaluatePostfix(postfix));

    printf("Expression Tree Evaluation      = %d\n",
           evaluateTree(root));

    return 0;
}
