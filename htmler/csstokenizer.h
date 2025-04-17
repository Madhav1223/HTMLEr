#pragma once	
#include<iostream>
#include<stack>
#include<vector>
#define endl '\n'
using std::cout;

enum class CssTokenState {
	INITIAL,
	BEFORE_SELECTOR_NAME,
	SELECTOR_NAME,
	AFTER_SELECTOR_NAME,
	BEFORE_DECLARATION,
	DECLARATION,
	COMMENT_STATE
};
enum class ReturnType {
	SELECTOR,
	DECLARATION,
	END_OF_FILE
};
typedef  std::pair<ReturnType, std::string> CssToken;
class CssTokenizer {
private:
	std::string currToken;
	ReturnType returnType;
	CssTokenState tokenState;
	bool reconsume;
	bool returnToken;
	void reset();
public:
	CssTokenizer();
	CssToken getNextToken(const std::string& str, int& currPosition);
};
