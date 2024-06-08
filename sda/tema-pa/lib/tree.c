#include "../include/tree.h"
#include "../include/game.h"
#include <stdio.h>
#include <string.h>

TreeNode* createTreeNode(Team* team) {
	TreeNode* node = calloc(1, sizeof(TreeNode));
	node->data = team;
	return node;
}

TreeNode* insertTreeNodeInBST(TreeNode* root, Team* team) {
	if (root == NULL)
		return createTreeNode(team);

	Team* rootTeam = root->data;

	if (team->points > rootTeam->points)
		root->left = insertTreeNodeInBST(root->left, team);
	else if (team->points == rootTeam->points && strcmp(team->name, rootTeam->name) > 0)
		root->left = insertTreeNodeInBST(root->left, team);
	else
		root->right = insertTreeNodeInBST(root->right, team);

	return root;
}

void printBST(FILE* outputFileStream, TreeNode* root) {
	if (root == NULL)
		return;

	Team* rootTeam = root->data;

	printBST(outputFileStream, root->left);
	fprintf(outputFileStream, "%-33s -  %.2f\n", rootTeam->name, rootTeam->points);
	printBST(outputFileStream, root->right);
}

int getHeight(TreeNode* root) {
	if (root == NULL)
		return 0;
	return root->height;
}

void updateHeight(TreeNode* root) {
	int leftTreeHeight = getHeight(root->left);
	int rightTreeHeight = getHeight(root->right);

	if (leftTreeHeight > rightTreeHeight)
		root->height = leftTreeHeight + 1;
	else
		root->height = rightTreeHeight + 1;
}

int getBalanceFactor(TreeNode* root) {
	if (root == NULL)
		return 0;
	return getHeight(root->left) - getHeight(root->right);
}

TreeNode* rotateLeft(TreeNode* root) {
	TreeNode* aux = root->right;
	root->right = aux->left;
	aux->left = root;
	updateHeight(root);
	updateHeight(aux);
	return aux;
}

TreeNode* rotateRight(TreeNode* root) {
	TreeNode* aux = root->left;
	root->left = aux->right;
	aux->right = root;
	updateHeight(root);
	updateHeight(aux);
	return aux;
}

TreeNode* insertTreeNodeInAVL(TreeNode* root, Team* team) {
	if (root == NULL)
		return createTreeNode(team);

	Team* rootTeam = root->data;

	if (team->points > rootTeam->points)
		root->left = insertTreeNodeInAVL(root->left, team);
	else if (team->points == rootTeam->points && strcmp(team->name, rootTeam->name) > 0)
		root->left = insertTreeNodeInAVL(root->left, team);
	else
		root->right = insertTreeNodeInAVL(root->right, team);

	updateHeight(root);
	int balanceFactor = getBalanceFactor(root);

	Team* leftTeam;
	Team* rightTeam;

	if (root->left != NULL)
		leftTeam = root->left->data;
	if (root->right != NULL)
		rightTeam = root->right->data;

	if (balanceFactor > 1)
		if (team->points < leftTeam->points)
			return rotateRight(root);

	if (balanceFactor < -1)
		if (team->points < rightTeam->points)
			return rotateLeft(root);

	return root;
}

void printLevelTwoFromAVL(FILE* outputFileStream, TreeNode* root, int* index) {
	if (root == NULL)
		return;

	printLevelTwoFromAVL(outputFileStream, root->left, index);

	if (*index % 2 == 0) {
		Team* team = root->data;
		fprintf(outputFileStream, "%s\n", team->name);
	}
	*index = *index + 1;

	printLevelTwoFromAVL(outputFileStream, root->right, index);
}
