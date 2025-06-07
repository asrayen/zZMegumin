#include <iostream>
#include <string.h>
#include <algorithm>
#define maxn 105
using namespace std;

typedef struct TreeNode
{
    TreeNode(int key) : key(key), height(1), left(nullptr), right(nullptr) {}
    int key;
    int height;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 右旋
TreeNode *rightRotate(TreeNode *root)
{
    TreeNode *left = root->left;
    root->left = left->right;
    left->right = root;
    root->height = max((root->left != nullptr ? root->left->height : 0),
                       (root->right != nullptr ? root->right->height : 0)) +
                   1;
    left->height = max((left->left != nullptr ? left->left->height : 0),
                       root->height) +
                   1;
    return left;
}
// 左旋
TreeNode *leftRotate(TreeNode *root)
{
    TreeNode *right = root->right;
    root->right = right->left;
    right->left = root;
    root->height = max((root->left != nullptr ? root->left->height : 0),
                       (root->right != nullptr ? root->right->height : 0)) +
                   1;
    right->height = max(root->height,
                        (right->right != nullptr ? right->right->height : 0)) +
                    1;
    return right;
}

TreeNode *maintainTreeNode(TreeNode *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    int left_height = (root->left != nullptr) ? root->left->height : 0;
    int right_height = (root->right != nullptr) ? root->right->height : 0;
    if (abs(left_height - right_height) > 1)
    {
        if (left_height > right_height)
        {
            int left_left_height = (root->left != nullptr) && (root->left->left != nullptr) ? root->left->left->height : 0;
            int left_right_height = (root->left != nullptr) && (root->left->right != nullptr) ? root->left->right->height : 0;
            if (left_left_height >= left_right_height)
            {
                root = rightRotate(root); // LL type
            }
            else
            {
                root->left = leftRotate(root->left);
                root = rightRotate(root); // LR type
            }
        }
        else
        {
            int right_left_height = (root->right != nullptr) && (root->right->left != nullptr) ? root->right->left->height : 0;
            int right_right_height = (root->right != nullptr) && (root->right->right != nullptr) ? root->right->right->height : 0;
            if (right_right_height >= right_left_height)
            {
                root = leftRotate(root); // RR type
            }
            else
            {
                root->right = rightRotate(root->right);
                root = leftRotate(root); // RL type
            }
        }
    }
    root->height = max((root->left != nullptr ? root->left->height : 0),
                       (root->right != nullptr ? root->right->height : 0)) +
                   1;
    return root;
}

TreeNode *insertTreeNode(TreeNode *root, int key)
{
    if (root == nullptr)
    {
        return new TreeNode(key);
    }
    else
    {
        if (root->key < key)
        {
            root->right = insertTreeNode(root->right, key);
        }
        else
        {
            root->left = insertTreeNode(root->left, key);
        }
        // 更新节点的height值
        root->height = max((root->left != nullptr ? root->left->height : 0), (root->right != nullptr ? root->right->height : 0)) + 1;
        return maintainTreeNode(root);
    }
}

TreeNode *searchTreeNode(TreeNode *root, int key)
{
    while (root != nullptr && root->key != key)
    {
        if (root->key < key)
        {
            root = root->right;
        }
        else
        {
            root = root->left;
        }
    }
    return root;
}

TreeNode *deleteTreeNode(TreeNode *root, int key)
{
    if (root->key < key)
    {
        root->right = deleteTreeNode(root->right, key);
    }
    else if (root->key > key)
    {
        root->left = deleteTreeNode(root->left, key);
    }
    else
    {
        // 左右都为空
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            return nullptr;
        }
        // 左不空右空
        else if (root->left != nullptr && root->right == nullptr)
        {
            TreeNode *temp = root->left;
            delete root;
            root = temp;
        }
        // 左空右不空
        else if (root->left == nullptr && root->right != nullptr)
        {
            TreeNode *temp = root->right;
            delete root;
            root = temp;
        }
        // 左右都不空
        else
        {
            // 寻找 左孩子中最大的 或者 右孩子中最小的
            TreeNode *temp = root->right;
            while (temp->left != nullptr)
            {
                temp = temp->left;
            }
            int key = temp->key;
            root->right = deleteTreeNode(root->right, key);
            root->key = key;
        }
    }

    if (root)
    {
        root->height = max((root->left != nullptr ? root->left->height : 0), (root->right != nullptr ? root->right->height : 0)) + 1;
    }

    return maintainTreeNode(root);
}

void searchForPredecessorAndSuccessor(TreeNode *root, int key, TreeNode **predecessor, TreeNode **successor)
{
    TreeNode *current = root;
    *predecessor = *successor = nullptr;

    while (current)
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
            TreeNode *pre = current->left;
            TreeNode *suc = current->right;
            while (pre->right != nullptr)
            {
                pre = pre->right;
            }
            *predecessor = pre;
            while (suc->left != nullptr)
            {
                suc = suc->left;
            }
            *successor = suc;
        }
    }
}
// 中序遍历(排序)
void inorder_traversal(TreeNode *root)
{
    if (root == nullptr)
        return;
    inorder_traversal(root->left);
    printf("%d %d\n", root->key, root->height);
    inorder_traversal(root->right);
}

int main()
{
    int n;
    scanf("%d", &n);
    TreeNode *root = NULL;
    for (int i = 1; i <= n; i++)
    {
        int key;
        scanf("%d", &key);
        root = insertTreeNode(root, key);
    }
    inorder_traversal(root);
    return 0;
}