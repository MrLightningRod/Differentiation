#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include <string.h>

const int _NODE_UNINIT_ = 0;
const int _NODE_VALUE_ = 1;
const int _NODE_OPERATOR_ = 2;
const int _NODE_VARIABLE_ = 3;
const size_t _D_SIZE_ = 20;

class TreeNode
{
private:
	int _data_type;
	int _val;
	char _data[_D_SIZE_];
	TreeNode *_left;
	TreeNode *_right;
public:
	TreeNode();
	~TreeNode();
	TreeNode(const TreeNode &that);
	TreeNode(int val);
	TreeNode(char *var);
	TreeNode(TreeNode *left, const char *oper, TreeNode *right);
	TreeNode(int left, const char *oper, int right);
	TreeNode(TreeNode *left, const char *oper, int right);
	int is_ok() const;
	int is_val() const;
	int val() const;
	void read_var(FILE *input);
	void read_oper(FILE *input);
	void simple_print(FILE *output) const;
	void tex_print(FILE *output) const;
	void del_l();
	void del_r();
	void del_this_r();
	void del_this_l();
	void precount();
	TreeNode *diff(const char * var);
};

TreeNode *read_tree(FILE *input);
void print_tex(TreeNode *tree, FILE *output);