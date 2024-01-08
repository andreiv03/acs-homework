#ifndef _TREE_H_
#define _TREE_H_

#include "./game.h"
#include <stdio.h>

typedef struct TreeNode {
	void* data;
	int height;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

TreeNode* createTreeNode(Team* team);
TreeNode* insertTreeNodeInBST(TreeNode* root, Team* team);
void printBST(FILE* outputFileStream, TreeNode* root);
int getHeight(TreeNode* root);
void updateHeight(TreeNode* root);
int getBalanceFactor(TreeNode* root);
TreeNode* rotateLeft(TreeNode* node);
TreeNode* rotateRight(TreeNode* root);
TreeNode* insertTreeNodeInAVL(TreeNode* root, Team* team);
void printLevelTwoFromAVL(FILE* outputFileStream, TreeNode* root, int* index);

#endif
