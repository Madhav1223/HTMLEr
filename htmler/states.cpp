#include <algorithm> 
#include <functional> 
#include <cctype>
#include <string>
#include <locale>
#include"states.h"

std::string getTokenType(TokenType t) {
	switch (t) {
	case CHARACTER:return "CHARACTER";
	case TAG:return "TAG";
	case TOK_COMMENT:return "COMMENT";
	default: return "OTHER";
	}
}

inline std::string& ltrim(std::string& s) {
	//https://stackoverflow.com/a/44973498/16531138
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); }));
	return s;
}

// trim from end (in place)
inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}
void trim(std::string& s) {
	s = ltrim(s);
	rtrim(s);
}

TagName getTagNameAsEnum(const std::string& ss) {
	std::string s{ ss };
	ltrim(s);
	if (s == "html")return HTML;
	if (s == "head")return HEAD;
	if (s == "body")return BODY;
	if (s == "title")return TITLE;
	if (s == "div")return DIV;
	if (s == "h1")return H1;
	if (s == "h2")return H2;
	if (s == "h3")return H3;
	if (s == "h4")return H4;
	if (s == "h5")return H5;
	if (s == "h6")return H6;
	if (s == "li")return LI;
	if (s == "ul")return UL;
	if (s == "ol")return OL;
	if (s == "nav")return NAV;
	if (s == "br")return BR;
	if (s == "p")return P;
	if (s == "article")return ARTICLE;
	if (s == "hr")return HR;
	if (s == "em")return EM;
	if (s == "meta")return META;
	if (s == "button")return BUTTON;
	if (s == "span")return SPAN;

	return NOTAG;
}
std::string getTagName(TagName t) {

	if (t == HTML)return "html";
	if (t == HEAD)return "head";
	if (t == BODY)return "body";
	if (t == TITLE)return "title";
	if (t == DIV)return "div";
	if (t == H1)return "h1";
	if (t == H2)return "h2";
	if (t == H3)return "h3";
	if (t == H4)return "h4";
	if (t == H5)return "h5";
	if (t == H6)return "h6";
	if (t == LI)return "li";
	if (t == UL)return "ul";
	if (t == NAV)return "nav";
	if (t == BR)return "br";
}

