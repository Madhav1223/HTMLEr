#pragma once	
#include<iostream>
#include"csstokenizer.h"
#include"rendertree.h";
#include"csstypes.h"
#define endl '\n'
using std::cout;


class CssParser {
private:
	int currPosition;
	std::vector<CSSRule>cssRules;
	inline void ignoreWhiteSpace(const std::string& str, int& pos);
public:
	CssParser();
	void parse(const std::string& str);
	Declaration parsePropValue(const std::string& propValue);
	friend void RenderTree::addStyle(const CssParser&);
};
