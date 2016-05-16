#include "DiffTree.h"

TreeNode::TreeNode() : _data_type(_NODE_UNINIT_), _val(0), _left(NULL), _right(NULL)
{
}

TreeNode::~TreeNode()
{
	assert(this != NULL);
	_data_type = _NODE_UNINIT_;
	_val = 0;
	if (_left != NULL)
	{
		delete _left;
	}
	if (_right != NULL)
	{
		delete _right;
	}
}

TreeNode::TreeNode(const TreeNode &that) : _data_type(that._data_type), _val(that.val()), _left(NULL), _right(NULL)
{
	if (!that.is_val())
	{
		size_t i = 0;
		for (i = 0; i < _D_SIZE_; i++)
		{
			_data[i] = that._data[i];
		}
	}
	if (that._left != NULL)
	{
		_left = new TreeNode(*that._left);
	}
	if (that._right != NULL)
	{
		_right = new TreeNode(*that._right);
	}
}

TreeNode::TreeNode(int val) : _data_type(_NODE_VALUE_), _val(val), _left(NULL), _right(NULL)
{
}

TreeNode::TreeNode(char *var) : _data_type(_NODE_VARIABLE_), _val(0), _left(NULL), _right(NULL)
{
	size_t i = 0;
	do
	{
		assert(i < _D_SIZE_);
		_data[i] = var[i];
		i++;
	} while (var[i] != '\0');
}

TreeNode::TreeNode(TreeNode *left, const char *oper, TreeNode *right) : _data_type(_NODE_OPERATOR_), _left(left), _right(right), _val(0)
{
	size_t i = 0;
	while (oper[i] != '\0')
	{
		assert(i < _D_SIZE_);
		_data[i] = oper[i];
		i++;
	}
	_data[i] = '\0';
}

TreeNode::TreeNode(int left, const char *oper, int right) : _data_type(_NODE_OPERATOR_), _left(new TreeNode(left)), _right(new TreeNode(right)), _val(0)
{
	size_t i = 0;
	while (oper[i] != '\0')
	{
		assert(i < _D_SIZE_);
		_data[i] = oper[i];
		i++;
	}
	_data[i] = '\0';
}

TreeNode::TreeNode(TreeNode *left, const char *oper, int right) : _data_type(_NODE_OPERATOR_), _left(left), _right(new TreeNode(right)), _val(0)
{
	size_t i = 0;
	while (oper[i] != '\0')
	{
		assert(i < _D_SIZE_);
		_data[i] = oper[i];
		i++;
	}
	_data[i] = '\0';
}

int TreeNode::is_ok() const
{
	return ((this != NULL) &&(_data_type >= _NODE_UNINIT_) && (_data_type <= _NODE_VARIABLE_));
}

int TreeNode::is_val() const
{
	assert(this->is_ok());
	return (_data_type == _NODE_VALUE_);
}

int TreeNode::val() const
{
	assert(this->is_ok());
	return _val;
}

void TreeNode::del_l()
{
	assert(this != NULL);
	if (_left != NULL)
	{
		delete _left;
		_left = NULL;
	}
}

void TreeNode::del_r()
{
	assert(this != NULL);
	if (_right != NULL)
	{
		delete _right;
		_right = NULL;
	}
}

void TreeNode::del_this_l()
{
	assert(this->is_ok());
	assert(_right->is_ok());
	this->del_l();
	_data_type = _right->_data_type;
	_val = _right->_val;
	size_t i = 0;
	while (_right->_data[i] != '\0')
	{
		_data[i] = _right->_data[i];
		i++;
	}
	_data[i] = '\0';
	TreeNode *temp = _right;
	_left = temp->_left;
	_right = temp->_right;
	temp->_left = NULL;
	temp->_right = NULL;
	delete temp;
}

void TreeNode::del_this_r()
{
	assert(this->is_ok());
	assert(_left->is_ok());
	this->del_r();
	_data_type = _left->_data_type;
	_val = _left->_val;
	size_t i = 0;
	if (_data_type != _NODE_VALUE_)

	{
		while (_left->_data[i] != '\0')
		{
			_data[i] = _left->_data[i];
			i++;
		}
	}
	_data[i] = '\0';
	TreeNode *temp = _left;
	_left = temp->_left;
	_right = temp->_right;
	temp->_left = NULL;
	temp->_right = NULL;
	delete temp;
}

void TreeNode::read_var(FILE *input)
{
	assert(input != NULL);
	char inp = 0;
	size_t i = 0;
	while ((fscanf_s(input, "%c", &inp, 1) == 1) && (inp != ']'))
	{
		assert(i < _D_SIZE_ - 1);
		_data[i] = inp;
		i++;
	}
	_data[i] = '\0';
	if ((_data[0] >= '0') && (_data[0] <= '9'))
	{
		_data_type = _NODE_VALUE_;
		sscanf_s(_data, "%d", &_val);
	}
	else
	{
		_data_type = _NODE_VARIABLE_;
	}
}

void TreeNode::read_oper(FILE *input)
{
	assert(input != NULL);
	char inp = 0;
	size_t i = 0;
	_data_type = _NODE_OPERATOR_;
	fscanf(input, "%c", &inp);
	if ((inp == '(') || (inp == '['))
	{
		_left = new TreeNode;
		if (inp == '(')
		{
			_left->read_oper(input);
		}
		else
		{
			_left->read_var(input);
		}
	}
	else
	{
		_data[i] = inp;
		i++;
	}
	while ((fscanf(input, "%c", &inp) == 1) && (inp != '(') && (inp != ')') && (inp != '['))
	{
		assert(i < _D_SIZE_ - 1);
		_data[i] = inp;
		i++;
	}
	_data[i] = '\0';
	switch (inp)
	{
	default:
		assert(inp == ')');
		break;
	case '(':
		_right = new TreeNode;
		_right->read_oper(input);
		fscanf(input, "%c", &inp);
		assert(inp == ')');
		break;
	case '[':
		_right = new TreeNode;
		_right->read_var(input);
		fscanf(input, "%c", &inp);
		assert(inp == ')');
		break;
	}
}

TreeNode *read_tree(FILE *input)
{
	assert(input != NULL);
	char inp = 0;
	fscanf(input, "%c", &inp);
	assert(inp == '(');
	TreeNode *tree = new TreeNode;
	tree->read_oper(input);
	return tree;
}

void TreeNode::simple_print(FILE *output) const
{
	if (_data_type == _NODE_OPERATOR_)
	{
		fprintf(output, "(");
		if (_left != NULL)
		{
			_left->simple_print(output);
		}
		fprintf(output, "%s", _data);
		if (_right != NULL)
		{
			_right->simple_print(output);
		}
		fprintf(output, ")");
	}
	else
	{
		fprintf(output, "[");
		if (_data_type == _NODE_VALUE_)
		{
			fprintf(output, "%d", _val);
		}
		else
		{
			fprintf(output, "%s", _data);
		}
		fprintf(output, "]");
	}
}

void print_tex(TreeNode *tree, FILE *output)
{
	assert(tree != NULL);
	assert(output != NULL);
	fprintf(output, "\\documentclass[10pt]{article}\n");
	fprintf(output, "\\usepackage{amssymb,amsmath,amsfonts,latexsym,mathtext}\n\n");
	fprintf(output, "\\begin{document}\n\n$");
	tree->tex_print(output);
	fprintf(output, "$\n\\end{document}\n");
}

void TreeNode::tex_print(FILE *output) const
{
	assert(this->is_ok());
	assert(output != NULL);
	if (_data_type == _NODE_VALUE_)
	{
		fprintf(output, "%d", _val);
	}
	if (_data_type == _NODE_VARIABLE_)
	{
		fprintf(output, "%s", _data);
	}
	if (_data_type == _NODE_OPERATOR_)
	{
		if ((strcmp(_data, "+") == 0) || (strcmp(_data, "-") == 0) || (strcmp(_data, "*") == 0))
		{
			if (_left != NULL)
			{
				if (_left->_data_type == _NODE_OPERATOR_)
				{
					fprintf(output, "(");
				}
				_left->tex_print(output);
				if (_left->_data_type == _NODE_OPERATOR_)
				{
					fprintf(output, ")");
				}
			}
			fprintf(output, "%s", _data);
			if (_right != NULL)
			{
				if (_right->_data_type == _NODE_OPERATOR_)
				{
					fprintf(output, "(");
				}
				_right->tex_print(output);
				if (_right->_data_type == _NODE_OPERATOR_)
				{
					fprintf(output, ")");
				}
			}
		}
		if ((strcmp(_data, "sin") == 0) || (strcmp(_data, "cos") == 0) || (strcmp(_data, "tan") == 0) || (strcmp(_data, "cot") == 0) || (strcmp(_data, "ln") == 0) || (strcmp(_data, "arcsin") == 0) || (strcmp(_data, "arccos") == 0) || (strcmp(_data, "arctan") == 0) || (strcmp(_data, "sinh") == 0) || (strcmp(_data, "cosh") == 0) || (strcmp(_data, "tanh") == 0))
		{
			fprintf(output, "\\%s(", _data);
			assert(_right != NULL);
			_right->tex_print(output);
			fprintf(output, ")");
		}
		if (strcmp(_data, "/") == 0)
		{
			assert(_left != NULL);
			fprintf(output, "\\frac{");
			_left->tex_print(output);
			fprintf(output, "}{");
			assert(_right != NULL);
			_right->tex_print(output);
			fprintf(output, "}");
		}
		if (strcmp(_data, "^") == 0)
		{
			assert(_left != NULL);
			if (_left->_data_type == _NODE_OPERATOR_)
			{
				fprintf(output, "(");
			}
			_left->tex_print(output);
			if (_left->_data_type == _NODE_OPERATOR_)
			{
				fprintf(output, ")");
			}
			fprintf(output, "^{");
			assert(_right != NULL);
			_right->tex_print(output);
			fprintf(output, "}");
		}
	}
}

TreeNode* TreeNode::diff(const char * var)
{
	assert(this->is_ok());
	TreeNode *rght = NULL;
	TreeNode *result = NULL;
	if (_data_type == _NODE_VALUE_)
	{
		return new TreeNode(0);
	}
	if (_data_type == _NODE_VARIABLE_)
	{
		if (strcmp(_data, var) == 0)
		{
			return new TreeNode(1);
		}
		else
		{
			return new TreeNode(0);
		}
	}
	if (_data_type == _NODE_OPERATOR_)
	{
		if (strcmp(_data, "+") == 0)
		{
			return new TreeNode(((_left == NULL) ? NULL : _left->diff(var)), "+", ((_right == NULL) ? NULL : _right->diff(var)));
		}
		if (strcmp(_data, "-") == 0)
		{
			return new TreeNode(((_left == NULL) ? NULL : _left->diff(var)), "-", ((_right == NULL) ? NULL : _right->diff(var)));
		}
		if (strcmp(_data, "*") == 0)
		{
			result = new TreeNode(_left->diff(var), "*", new TreeNode(*_right));
			rght = new TreeNode(new TreeNode(*_left), "*", (_right->diff(var)));
			return new TreeNode(result, "+", rght);
		}
		if (strcmp(_data, "/") == 0)
		{
			result = new TreeNode(_left->diff(var), "*", new TreeNode(*_right));
			rght = new TreeNode(new TreeNode(*_left), "*", (_right->diff(var)));
			result = new TreeNode(result, "-", rght);
			rght = new TreeNode(new TreeNode(*_right), "^", 2);
			return new TreeNode(result, "/", rght);
		}
		if (strcmp(_data, "sin") == 0)
		{
			rght = new TreeNode(NULL, "cos", new TreeNode(*_right));
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "cos") == 0)
		{
			rght = new TreeNode(NULL, "sin", new TreeNode(*_right));
			rght = new TreeNode(NULL, "-", rght);
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "tan") == 0)
		{
			rght = new TreeNode(NULL, "cos", new TreeNode(*_right));
			rght = new TreeNode(rght, "^", 2);
			return new TreeNode(_right->diff(var), "/", rght);
		}
		if (strcmp(_data, "cot") == 0)
		{
			rght = new TreeNode(NULL, "sin", new TreeNode(*_right));
			rght = new TreeNode(rght, "^", 2);
			result = new TreeNode(_right->diff(var), "/", rght);
			return new TreeNode(NULL, "-", result);
		}
		if (strcmp(_data, "ln") == 0)
		{
			return new TreeNode(_right->diff(var), "/", new TreeNode(*_right));
		}
		if (strcmp(_data, "^") == 0)
		{
			rght = new TreeNode(NULL, "ln", new TreeNode(*_left));
			rght = new TreeNode(rght, "*", new TreeNode(*_right));
			result = new TreeNode(new TreeNode(*this), "*", rght->diff(var));
			delete rght;
			return result;
		}
		if (strcmp(_data, "arcsin") == 0)
		{
			rght = new TreeNode(new TreeNode(*_right), "^", 2);
			rght = new TreeNode(new TreeNode(1), "-", rght);
			rght = new TreeNode(rght, "^", new TreeNode(new TreeNode(1), "/", 2));
			rght = new TreeNode(new TreeNode(1), "/", rght);
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "arccos") == 0)
		{
			rght = new TreeNode(new TreeNode(*_right), "^", 2);
			rght = new TreeNode(new TreeNode(1), "-", rght);
			rght = new TreeNode(rght, "^", new TreeNode(new TreeNode(1), "/", 2));
			rght = new TreeNode(new TreeNode(-1), "/", rght);
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "arctan") == 0)
		{
			rght = new TreeNode(new TreeNode(*_right), "^", 2);
			rght = new TreeNode(rght, "+", 1);
			rght = new TreeNode(new TreeNode(1), "/", rght);
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "sinh") == 0)
		{
			rght = new TreeNode(NULL, "cosh", new TreeNode(*_right));
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "cosh") == 0)
		{
			rght = new TreeNode(NULL, "sinh", new TreeNode(*_right));
			return new TreeNode(_right->diff(var), "*", rght);
		}
		if (strcmp(_data, "tanh") == 0)
		{
			rght = new TreeNode(NULL, "cosh", new TreeNode(*_right));
			rght = new TreeNode(rght, "^", 2);
			return new TreeNode(_right->diff(var), "/", rght);
		}
	}
	return NULL;
}

void TreeNode::precount()
{
	assert(this->is_ok());
	if (_data_type != _NODE_OPERATOR_)
	{
		return;
	}
	if (_left != NULL)
	{
		_left->precount();
	}
	if (_right != NULL)
	{
		_right->precount();
	}
	if ((strcmp(_data, "+") == 0) || (strcmp(_data, "-") == 0))
	{
		if ((_left != NULL) && (_left->is_val()) && (_left->_val == 0))
		{
			if (strcmp(_data, "+") == 0)
			{
				this->del_this_l();
			}
			else
			{
				this->del_l();
			}
			return;
		}
		if ((_right != NULL) && (_right->is_val()) && (_right->_val == 0))
		{
			this->del_this_r();
			return;
		}
		if ((_left != NULL) && (_right != NULL))
		{
			if ((_left->is_val()) && (_right->is_val()))
			{
				_data_type = _NODE_VALUE_;
				if (strcmp(_data, "+") == 0)
				{
					_val = _left->_val + _right->_val;
				}
				else
				{
					_val = _left->_val - _right->_val;
				}
				this->del_l();
				this->del_r();
				return;
			}
		}
	}
	if (strcmp(_data, "*") == 0)
	{
		if ((_left != NULL) && (_left->is_val()))
		{
			if (_left->_val == 0)
			{
				this->del_this_r();
				return;
			}
			if (_left->_val == 1)
			{
				this->del_this_l();
				return;
			}
		}
		if ((_right != NULL) && (_right->is_val()))
		{
			if (_right->_val == 0)
			{
				this->del_this_l();
				return;
			}
			if (_right->_val == 1)
			{
				this->del_this_r();
				return;
			}
		}
		if ((_left != NULL) && (_right != NULL))
		{
			if ((_left->is_val()) && (_right->is_val()))
			{
				_data_type = _NODE_VALUE_;
				_val = _left->_val * _right->_val;
				return;
			}
		}
	}
	if (strcmp(_data, "/") == 0)
	{
		if ((_left != NULL) && (_left->is_val()))
		{
			if (_left->_val == 0)
			{
				this->del_l();
				this->del_r();
				_data_type = _NODE_VALUE_;
				_val = 0;
				return;
			}
		}
		if ((_right != NULL) && (_right->is_val()))
		{
			if (_right->_val == 1)
			{
				this->del_this_r();
				return;
			}
		}
	}
}