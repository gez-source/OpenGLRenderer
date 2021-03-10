// Gerallt © 2017.

#include <iostream>
#include <fstream>
#include <sstream>
#include "XmlParser.h"
#include "xh_scanner.h"
#include <locale>
#include <codecvt>
//#include <QString>

using namespace std;
using namespace markup;

#define PADDING_MULTIPLIER 4

XmlParser::XmlParser() {}

// Parse XML from the specified filename.
void XmlParser::Parse(string fileName) 
{
	string xmlText;
	ifstream* ifs = new ifstream(fileName, std::ifstream::in);

	// READ
	if (ifs->is_open())
	{
		string line;
		stringstream ss;

		ss << ifs->rdbuf();

		xmlText = std::string(ss.str());
	}

	ifs->close();

	delete ifs;

	if (xmlText.size() == 0) 
	{
		cout << "level file returned no data" << endl;

		return;
	}

	// PARSE
#ifdef USE_RAPIDXML
	parseXMLRapid(&xmlText); // the old rapidxml parser that is having bad days
#else
	parseXmlBuiltIn(&xmlText); // my own implementation using a pull-parser technique adapted from Andrew Fedoniouk's xh_scanner
#endif
}

// Create a sequence of padding characters given the specified length and character
string XmlParser::padding(string padding, int paddingLength)
{
	string padded = string();
	int i;
	char* first = &padding.at(0);

	for (i = 0; i < paddingLength; i++)
	{
		padded.append(first);
	}

	return padded;
}

// Parse XML from the specified xml string. (using inbuilt library)
void XmlParser::parseXmlBuiltIn(string* xml)
{
	XmlStream si = XmlStream(&xml->at(0));
	XmlScanner sc = XmlScanner(si);;
	XmlToken t;
	int level;
	int depthPost;
	int depthPre;
	long order;
	long elemOrder;
	string spacing, spacing2, spacing3, spacing4;
	Stack work_items; // stack to maintain context of XmlScanner in tree
	dom_node* node;
	CXPathNode* xpNode;
	dom_node* docDomNode;
	string textNode;

	size_t tmp;

	// Push-pull XML/html parser
	// Optimised to a single pass through with full XPath capability, boo yeah.

	// the owner document documentOwner will be later parsed as an automatic result from the aggregation of all TT_TAG_END tokens.
	documentOwner = new CXPathNode(); 
	documentOwner->SetNodeType(XPathNodeType::DOCUMENT_NODE);
	
	while (!sc.Eof())
	{
		depthPre = sc.GetDepth();

		t = sc.GetToken();

		elemOrder = sc.GetElemOrder();
		depthPost = sc.GetDepth();
		order = sc.GetOrder();

		if (t == XmlToken::TT_TAG_START)
		{
			level = depthPre;
		}
		else if (XmlToken::TT_TAG_END)
		{
			level = depthPost;
		}

		spacing = padding("*", level * PADDING_MULTIPLIER);
		spacing2 = padding("\t", level);
		spacing3 = spacing.append(padding("\t", level));
		spacing4 = padding(" ", level);

		switch (t)
		{
		case XmlToken::TT_ERROR:
			trace(spacing3, "ERROR", " error with dom fragment");

			break;
		case XmlToken::TT_TAG_START:

			trace(spacing3, "{ ", sc.GetTagName());

			// CREATE NODE
			createNode(sc.GetTagName(), level, elemOrder, order, &work_items);

			break;
		case XmlToken::TT_TAG_END:

			trace(spacing3, "} ", sc.GetTagName());

#ifdef DEBUG_INTERMEDIATES
			work_items.Print([](void* o) -> string
			{
				stringstream ss;
				dom_node* n = (dom_node*)o;
				ss << n->order;
				ss << "<" << n->name << ">";
				return ss.str();
			});
#endif

			break;
		case XmlToken::TT_ATTR:
			
			createAttribute(order, sc.GetAttrName(), sc.GetValue(), &work_items);

			trace(spacing3, "  -->", sc.GetAttrName(), "=", sc.GetValue());

			break;
		case XmlToken::TT_WORD:
			textNode = sc.GetValue();
			
			createTextNode(order, &textNode, &work_items);

			trace(spacing3, "  -->TEXTNODE:<", textNode, ">");
			
			break;
		case XmlToken::TT_SPACE:
			//trace(spacing3, "SPACING:", sc.GetValue());

			break;
		}
	}

	// PARSE COMPLETE
	_input = new CXPathSequence();
	_input->AppendItem(documentOwner);
}

// Build a node and append to a DOM tree
void XmlParser::createNode(string qName, long level, long elemOrder, long order, Stack* context)
{
	dom_node* node = new dom_node();
	node->order = elemOrder;
	node->name = qName;
	node->level = level;

	if (level > 0)
	{
		node->xpath = new CXPathNode(); // have to create now and use later to maintain single pass 
	}
	else
	{
		node->xpath = documentOwner;
	}

	CText xsType("xs:anyType");    
	CText ns;
    if(XmlBaseNamespace == nullptr){
        CText empty("");
        ns = empty;
    }
    else {
        CText nameSpace(&XmlBaseNamespace->at(0));
        ns = nameSpace;
    }
	CText qname(&qName.at(0));
	node->xpath->SetQName(qname);
	node->xpath->SetNodeType(XPathNodeType::ELEMENT_NODE);
	node->xpath->SetNamespace(ns);
	node->xpath->SetXSDType(xsType);

	// setup relationships now while we have context so we can maintain a single parser pass

	dom_node *dnNode, *dnParent;
	CXPathNode *xpNode, *parent;
	if (context->IsEmpty())
	{
		// presumably the node is a root node and has no parent.
		parent = nullptr;
		dnParent = nullptr;
	}
	else
	{
		// Pull context of tokeniser from stack to find who the parent node should be

		Stack::node* previous = context->PeekNode();
		bool atParent = false;

		dnParent = (dom_node*)context->Peek();

		while (previous != nullptr && !atParent)
		{
			dnParent = (dom_node*)previous->data;
			
			atParent = dnParent->level < level;
			
			previous = previous->prev;
		}

		parent = dnParent->xpath;
		node->parent = dnParent;
		node->xpath->SetParent(parent);
		
		parent->AppendChild(node->xpath);
	}

	context->Push(node);


#ifdef DEBUG_INTERMEDIATES
	context->Print([](void* o) -> string
	{
		stringstream ss;
		dom_node* n = (dom_node*)o;
		ss << n->order;
		ss << "<" << n->name << ">";
		return ss.str();
	});
#endif
}

// Create an attribute and append to the DOM tree.
void XmlParser::createAttribute(long order, string name, string value, Stack* context)
{
	dom_node *dnNode;
	CXPathNode *xpNode, *parent;

	// Get the current node
	dnNode = ((dom_node*)context->Peek());
	xpNode = dnNode->xpath;
	parent = xpNode;

	// Update attributes in node
	CXPathNode *n;
	CText ct_qName(&name[0]);
	CText ct_value (&value.at(0));
	CText ns;
    if(XmlBaseNamespace == nullptr){
        CText empty ("");
        ns = empty;
    }
    else{
        CText nameSpace(&XmlBaseNamespace->at(0));
        ns = nameSpace;
    }
    CText sXSDType ("XS:anySimpleType");
	n = new CXPathNode(ct_value, sXSDType, order, documentOwner, ns, XPathNodeType::ATTRIBUTE_NODE, ct_qName, parent);
	
	// Append to DOM
	xpNode->AppendAttribute(n);
}

void XmlParser::createTextNode(long order, string* value, Stack* context)
{
	dom_node *dnNode;
	CXPathNode *xpNode, *parent;

	// Get the current node
	dnNode = ((dom_node*)context->Peek());
	xpNode = dnNode->xpath;
	parent = xpNode;

	// Create a text node
	CXPathNode *n;
	CText ct_value (&value->at(0));
	CText ns;
    if(XmlBaseNamespace == nullptr){
        CText empty("");
        ns = empty;
    }
    else {
        CText nameSpace(&XmlBaseNamespace->at(0));
        ns = nameSpace;
    }
    CText sXSDType ("XS:anySimpleType");
    CText empty("");
	n = new CXPathNode(ct_value, sXSDType, order, documentOwner, ns, XPathNodeType::TEXT_NODE, empty, parent);
	
	xpNode->SetStringValue(ct_value);

	// Append to DOM
	xpNode->AppendChild(n);
}

// Creates an XPath node with the specified settings.
CXPathNode* XmlParser::createXpNode(string qName, const string* value, XPathNodeType type, int level, int elemOrder, int order, CXPathNode* owner, CXPathNode* parent)
{
	CXPathNode* n;
	CText xsType ("xs:anyType");
	CText ns;
    if(XmlBaseNamespace == nullptr){
        CText empty("");
        ns = empty;
    }
    else{
        CText nameSpace (&XmlBaseNamespace->at(0));
        ns = nameSpace;
    }
	CText qname (&qName.at(0));
	CText val;

    if(value == nullptr || (*value) == ""){
        CText empty2("");
        val = empty2;
    }
    else {
        CText value2(&((*value)[0]));
        val = value2;
    }

	if (level > 0)
	{
		n = new CXPathNode(val, xsType, order, owner, ns, type, qname, parent);
	}
	else
	{
		n = owner;
		n->SetStringValue(val);
		n->SetXSDType(xsType);
		n->SetNamespace(ns);
		n->SetNodeType(type);
		n->SetDocumentOrder(0);
		n->SetQName(qname);
		n->SetParent(nullptr);
		n->UpdateOwnerDocument(nullptr);
	}

	

	return n;
}

string XmlParser::Query(string xpathExpression)
{
	CXPathSequence* result;

	result = Expr(xpathExpression);

	return result->ToString();
}

CXPathSequence* XmlParser::Expr(string xpathExpression) 
{
	if (_input == nullptr) return nullptr;

	CXPathSequence* result = new CXPathSequence();
	CText query (xpathExpression);

	result = DoXPath(_input, query);


	return result;
}

CXPathSequence* XmlParser::Expr(CXPathSequence* input, string xpathExpression)
{
	if (input == nullptr) return nullptr;

	CXPathSequence * result = new CXPathSequence();
	CText query(xpathExpression);

	result = DoXPath(input, query);


	return result;
}

void XmlParser::trace(string m)
{
	if (debugging)
	{
		cout << m << endl;
	}
}

void XmlParser::trace(string m, string m2)
{
	if (debugging)
	{
		cout << m << m2 << endl;
	}
}

void XmlParser::trace(string m, string m2, string m3)
{
	if (debugging)
	{
		cout << m << m2 << m3 << endl;
	}
}

void XmlParser::trace(string m, string m2, string m3, string m4)
{
	if (debugging)
	{
		cout << m << m2 << m3 << m4 << endl;
	}
}

void XmlParser::trace(string m, string m2, string m3, string m4, string m5)
{
	if (debugging)
	{
		cout << m << m2 << m3 << m4 << m5 << endl;
	}
}

XmlParser::~XmlParser()
{
	delete _input;
}
