#ifndef EXPR_TREE
#define EXPR_TREE

#include "ExprTreeNode.h"




class CExprTree
{
public:
	CExprTree(void);
	virtual ~CExprTree(void);

	CExprTreeNode *Root;
	CExprTreeNode *CurrentNode;

	void SetLeftChild(CExprTreeNode*);
	void SetRightChild(CExprTreeNode*);

};
#endif
