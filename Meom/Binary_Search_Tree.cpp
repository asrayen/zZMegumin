#include <iostream>
#include <string.h>
#define ll long long int
#define maxn 500005
#define INF 0x7fffffff

typedef struct TreeNode
{
    int key;
    struct TreeNode *left, *right, *parent;
} TreeNode;

TreeNode *newTreeNode(int key)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node)
    {
        node->key = key;
        node->left = node->right = node->parent = NULL;
    }
    return node;
}

TreeNode *insertTreeNode(TreeNode *root, int key)
{
    TreeNode *y = NULL;
    TreeNode *x = root;

    while (x != NULL)
    {
        y = x;
        if (key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    TreeNode *newNode = newTreeNode(key);
    newNode->parent = y;

    if (y == NULL)
    {
        root = newNode;
    }
    else if (key < y->key)
    {
        y->left = newNode;
    }
    else if (key > y->key)
    {
        y->right = newNode;
    }
    else
    {
        printf("Already Exist\n");
    }
    return root;
}

TreeNode *searchTreeNode(TreeNode *root, int key)
{
    while (root != NULL && key != root->key)
    {
        if (key < root->key)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return root;
}

TreeNode *minTreeNode(TreeNode *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

TreeNode *maxTreeNode(TreeNode *node)
{
    while (node->right != NULL)
    {
        node = node->right;
    }
    return node;
}

TreeNode *treePredecessor(TreeNode *node)
{
    if (node->left != NULL)
    {
        return maxTreeNode(node->left);
    }
    TreeNode *p = node->parent;
    while (p != NULL && node == p->left)
    {
        node = p;
        p = p->parent;
    }
    return p;
}

TreeNode *treeSuccessor(TreeNode *node)
{
    if (node->right != NULL)
    {
        return minTreeNode(node->right);
    }
    TreeNode *p = node->parent;
    while (p != NULL && node == p->right)
    {
        node = p;
        p = p->parent;
    }
    return p;
}

TreeNode *deleteTreeNode(TreeNode *root, int key)
{
    TreeNode *node = searchTreeNode(root, key);
    if (!node)
        return root; // 如果节点不存在，直接返回

    TreeNode *x, *y;

    // 确定y是删除节点或其唯一孩子
    if (node->left == NULL || node->right == NULL)
    {
        y = node;
    }
    else
    {
        y = treeSuccessor(node);
    }

    // x是y的唯一孩子
    if (y->left != NULL)
    {
        x = y->left;
    }
    else
    {
        x = y->right;
    }

    // 将x链接到y的父节点
    if (x != NULL)
    {
        x->parent = y->parent;
    }

    if (y->parent == NULL)
    {
        root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    if (y != node)
    {
        node->key = y->key;
    }

    free(y);
    return root;
}

void searchForPredecessorAndSuccessor(TreeNode *root, int key, TreeNode **predecessor, TreeNode **successor)
{
    TreeNode *current = root;
    *predecessor = *successor = NULL;

    while (current != NULL)
    {
        if (current->key > key)
        {
            *successor = current;
            current = current->left;
        }
        else if (current->key < key)
        {
            *predecessor = current;
            current = current->right;
        }
        else
        {
            // 如果找到精确匹配，使用标准的前驱和后继查找
            if (current->left != NULL)
            {
                *predecessor = maxTreeNode(current->left);
            }
            if (current->right != NULL)
            {
                *successor = minTreeNode(current->right);
            }
            return;
        }
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    TreeNode *root = NULL;
    root = (TreeNode *)malloc(maxn * sizeof(TreeNode));
    for (int i = 1; i <= n; i++)
    {
        int opt, num;
        scanf("%d%d", &opt, &num);
        if (opt == 1)
        {
            root = insertTreeNode(root, num);
        }
        else
        {
            TreeNode *temp = searchTreeNode(root, num);
            if (temp != NULL)
            {
                printf("%d\n", temp->key);
                deleteTreeNode(root, num);
            }
            else
            {
                TreeNode *predecessor = NULL;
                TreeNode *successor = NULL;
                searchForPredecessorAndSuccessor(root, num, &predecessor, &successor);
                if (predecessor == NULL && successor != NULL)
                {
                    printf("%d\n", successor->key);
                    deleteTreeNode(root, successor->key);
                }
                else if (predecessor != NULL && successor == NULL)
                {
                    printf("%d\n", predecessor->key);
                }
                else
                {
                    int x = num - predecessor->key;
                    int y = successor->key - num;
                    if (x <= y)
                    {
                        printf("%d\n", predecessor->key);
                        deleteTreeNode(root, predecessor->key);
                    }
                    else
                    {
                        printf("%d\n", successor->key);
                        deleteTreeNode(root, successor->key);
                    }
                }
            }
        }
    }
    return 0;
}