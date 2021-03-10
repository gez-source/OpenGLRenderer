#ifndef _XPATHSEQUENCE_H
#define _XPATHSEQUENCE_H

#include "XPathItem.h"
#include <vector>

class CXPathSequence;
using namespace std;

//each node can be classified as follows
enum XPathNodeType {
	DOCUMENT_NODE, 
	ELEMENT_NODE, 
	ATTRIBUTE_NODE, 
	TEXT_NODE, 
	XML_NAMESPACE_NODE, 
	PROCESSING_INSTRUCTION_NODE, 
	COMMENT_NODE
};

struct XPathDocumentOrderSort {
     bool operator()(CXPathItem*& rpStart, CXPathItem*& rpEnd) {
		 return rpStart->GetDocumentOrder() < rpEnd->GetDocumentOrder();
     }
};

class CXPathSequence;
typedef CXPathSequence* XmlNode;

class CXPathSequence {
friend class CEvalEngine;
friend class CXPathNode;
public:
	CXPathSequence();
	virtual ~CXPathSequence();
private:
	vector<CXPathItem*> m_Items;
public:
	CXPathItem* GetItemAt(int i); // return child CXPathItem instance at index, i
	void AppendItem(CXPathItem *item);
	void ConcatSequence(CXPathSequence *other);
	//will add a number of sequence access methods.
	CXPathSequence* GetAtomicValues();
	CXPathSequence* GetAtomicValues(CText xsdType);
	CXPathSequence* GetNodesOfType(XPathNodeType nodeType);
	int GetSize(void);
	bool IsSingleton(void);
	bool IsEmpty(void);
	void SortItemsByDocOrder(void);
	string ToString(void);
	CXPathSequence* Expr(std::string query);
	std::string Query(std::string query);
	XmlNode GetChild(std::string elementName);
	XmlNode GetChildWithAttribute(std::string childName, std::string attr, std::string value);
	std::vector<XmlNode> GetChildren(std::string name);
	std::string GetAttribute(std::string attributeName);
	std::string GetElementName();
	std::string GetData();
};

#endif