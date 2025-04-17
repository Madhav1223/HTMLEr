#pragma once
#include<stack>
#include<string>
#include"states.h"
#include"tokenizer.h"
#include "node.h"
#define endl '\n'
class Parser {
private:
	std::stack<Element*> mstack;
	ParseState parseState;
	ParseState originalParseState;
	int currPosition;
	Document* mdocument;
	Element* headptr = nullptr;
	Element* createNewHtmlElement(TagName tn);

public:
	Parser(Document* doc);
	void parse(const std::string& str);
	void printStackOfOpenElements();
	void  create_element_for_token(const std::string& tn);
	void  generic_rcdata_parse(const std::string& tn);
	void insert_character(const std::string& ch);
	void create_element_for_token(const TagTokenizer& tt);
	~Parser() {
	}
};
