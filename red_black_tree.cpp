#include <iostream>
#include <cstring>
#include <algorithm>
#define maxn 105
#define red 1
#define black 0
using namespace std;

typedef struct TreeNode
{
    TreeNode(int key) : key(key), height(1), color(black), left(nullptr), right(nullptr), parent(nullptr) {}
    int key;
    int height;
    bool color;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
} TreeNode;

// 左根右 根叶黑 不红红 黑路同

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
// 查询叔叔节点
TreeNode *getUncle(TreeNode *root)
{
    TreeNode *null_black = new TreeNode(0);
    if (root->parent == nullptr || root->parent->parent == nullptr)
    {
        return null_black;
    }
    else
    {
        TreeNode *granpa = root->parent->parent;
        if (granpa->left == root && granpa->right != nullptr)
        {
            return granpa->right;
        }
        else if (granpa->right == root && granpa->left != nullptr)
        {
            return granpa->left;
        }
        else
        {
            return null_black;
        }
    }
}
// 维护
TreeNode *maintainTreeNode(TreeNode *root)
{
    if (root->color == red && root->parent == nullptr)
    {
        root->color = black;
    }
    if (root->parent->color == red)
    {
        TreeNode *uncle = getUncle(root);
        TreeNode *granpa = root->parent->parent;
        if (uncle->color == red) // 叔叔是红色
        {
            root->parent->color ^= 1;
            uncle->color ^= 1;
            granpa->color ^= 1;
            root = granpa;
            maintainTreeNode(root);
        }
        else // 叔叔是黑色
        {
            int left_height = granpa->left->height;
            int right_height = granpa->right->height;
            if (abs(left_height - right_height) > 1)
            {
                if (left_height > right_height)
                {
                }
            }
            else
            {
                printf("维护阶段-黑色叔叔-出错\n");
                return nullptr;
            }
        }
    }
}
// 插入
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

    TreeNode *newNode = new TreeNode(key);
    newNode->parent = y;
    newNode->color = red;
    if (y == NULL)
    {
        root = newNode;
    }
    else if (key < y->key)
    {
        y->left = newNode;
    }
    else
    {
        y->right = newNode;
    }
    root->height = max((root->left != nullptr ? root->left->height : 0),
                       (root->right != nullptr ? root->right->height : 0)) +
                   1;
    return root;
}