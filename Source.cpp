#include "DiffTree.h"//точка входа 

#define _INP_FILE_ "tree.in" 
#define _OUT_FILE_ "tree.out" 
#define _OUT_TEX_FILE_ "tree.tex" 

int main(int argc, char *argv[])
{
	FILE * input = NULL;
	if (argc < 2)
	{
		input = fopen(_INP_FILE_, "rb");
	}
	else
	{
		input = fopen(argv[1], "rb");
	}
	FILE *output = fopen(_OUT_FILE_, "wb");
	FILE *tex_out = fopen(_OUT_TEX_FILE_, "wb");
	TreeNode *tree = read_tree(input);
	tree->precount();
	TreeNode *result = NULL;
	if (argc > 2)
	{
		result = tree->diff(argv[2]);
	}
	else
	{
		result = tree->diff("x");
	}
	result->precount();
	result->simple_print(output);
	print_tex(result, tex_out);
	fclose(input);
	fclose(output);
	fclose(tex_out);
	delete tree;
	delete result;
	_CrtDumpMemoryLeaks();
	return 0;
}