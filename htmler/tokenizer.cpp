#pragma once
#include<iostream>
#include<cctype>
#include "tokenizer.h"
#define endl '\n'

Tokenizer::Tokenizer()
	:currState{ DATA },
	currToken{ "" },
	returnType{ CHARACTER },
	//currPosition{ 0 },
	shouldReturn{ false },
	increase{ true }{
}

void Tokenizer::reset() {
	increase = true;
	currToken = "";
	shouldReturn = false;
	returnType = CHARACTER;//default
}
bool isWordPresent(char* mainSting, int currPos, const char* strtoCheck) {
	bool flag = true;
	for (int i = currPos; i < currPos + strlen(strtoCheck); i++)
	{
		if (mainSting[i] != strtoCheck[i - currPos]) {
			flag = false;
		}
	}
	return flag;
}
struct Token Tokenizer::getNextToken(const std::string& str, int& currPosition) {

	reset();
	//TODO: handle ampersand(&), EOF and '\0'
	while (1) {
		//tokenizer
		if (currPosition >= str.length()) {
			if (currState == DATA) {
				return (Token{ .type = END_OF_FILE,.token = currToken });
			}
			else {
				//exit on encounter of EOF
				std::cout << "FIle ended in abnormal state" << endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (str[currPosition] == EOF)
		{
			currToken = END_OF_FILE;
			if (currState == DATA) {
				return (Token{ .type = END_OF_FILE,.token = currToken });
			}
			else {
				//exit on encounter of EOF
				std::cout << "FIle ended in abnormal state" << endl;
				exit(EXIT_FAILURE);
			}
		}
		switch (currState) {
		case DATA:
			switch (str[currPosition]) {
			case ' ':
			case '\n':
			case '\t':
			case '\f':
				//IllegalArgumentException

				if (currToken == "") {
					break;
				}
				if (currToken.back() == ' ')break;
				currToken.push_back(' ');
				break;
			case '&':
				currState = DATA;
				shouldReturn = true;
				//returnType=CHARACTER//default
				break;
			case '<':
				currState = TAG_OPEN;
				if (currToken.length() != 0) {
					std::string trimmedToken = ltrim(currToken);
					//if (currToken.compare(std::string(currToken.size(), ' ')) != 0) {
					if (trimmedToken == "") {
						currToken = "";
						break;
					}
					else {
						shouldReturn = true;
					}
					//if (currToken.compare(std::string(currToken.size(), ' ')) != 0) {
					//else {
						//currToken = "";
					//?/}
				}
				break;
			default:
				currToken.push_back(str[currPosition]);
				//shouldReturn = true;
				//returnType=CHARACTER//default
			}
			break;
		case TAG_OPEN:
			switch (str[currPosition]) {
			case '!':
				currState = MARK_UP_DECLARATION_OPEN;
				break;
			case '/':
				currState = END_TAG_OPEN;
				break;
			case '?':
				exit(EXIT_FAILURE);
				break;
			case EOF:
				currToken.push_back('<');
				shouldReturn = true;
				increase = false;
				break;
			default:
				if (isalpha(str[currPosition])) {
					currState = INSIDE_TAG;
					increase = false;
					break;
				}
				else {
					currToken.push_back('<');
					currState = DATA;
					increase = false;
					break;
				}
			}
			break;
		case END_TAG_OPEN:
			switch (str[currPosition]) {
			case '>':
				exit(EXIT_FAILURE);
				break;
			default:
				if (isalpha(str[currPosition])) {
					currState = TAG_NAME;
					increase = false;
					returnType = END_TAG;
					break;
				}
				else {
					currToken.push_back('>');
					currState = DATA;
					increase = false;
					break;
				}
			}
			break;
		case INSIDE_TAG:
			switch (str[currPosition]) {
			case '>':
				currState = DATA;
				currToken.push_back(str[currPosition]);
				shouldReturn = true;
				returnType = TAG;
				break;
			default:
				currToken.push_back(str[currPosition]);
				break;
			}
			break;
		case TAG_NAME:
			switch (str[currPosition]) {
			case '\t':
			case '\n':
			case '\f':
			case ' ':
				currState = BEFORE_ATTRIBUTE_NAME;
				break;
			case '/':
				currState = SELF_CLOSING_START_TAG;
				break;
			case '>':
				shouldReturn = true;
				currState = DATA;
				if (returnType != END_TAG) {
					returnType = TAG;
				}
				break;
			default:
				if (isupper(str[currPosition])) {
					currToken.push_back(tolower(str[currPosition]));
					break;

				}
				else {
					currToken.push_back(str[currPosition]);
					break;
				}
			}
			break;
		case RAWTEXT:
			switch (str[currPosition]) {
			case '<':
				currState = RAW_TEXT_LESS_THAN_SIGN;
			default:
				currToken.push_back(str[currPosition]);

			}
			break;
		case SELF_CLOSING_START_TAG:
			switch (str[currPosition]) {
			case '>':
				currState = DATA;
			default:
				exit(EXIT_FAILURE);
			}
			break;
		case MARK_UP_DECLARATION_OPEN:
			switch (str[currPosition]) {
			case '-':
				if (str[++currPosition] == '-') {
					currState = COMMENT_START;
				}
				break;
			default:
				if (strncmp(str.c_str(), "DOCTYPE", 7)) {
					currToken.append("DOCTYPE");
					//currState = DOCTYPE;
					while (str[currPosition] != '>') {
						//ignore doctype token
						currPosition++;
						currToken = "";
						currState = DATA;
					}
				}
				else if (strncmp(str.c_str(), "[CDATA[", 7)) {
					exit(EXIT_FAILURE);
				}
			}
			break;
			//else if (isWordPresent(cstr,currPosition,"[CDATA[")){
				//currToken.push_back(str[currPosition]);
//			}
		case DOCTYPE:
			switch (str[currPosition]) {
			case ' ':
			case '\t':
			case '\r':
			case '\f':
				currState = BEFORE_DOCTYPE_NAME;
			case '>':
				increase = false;
				currState = BEFORE_DOCTYPE_NAME;
				//missing doctype name error
			default:
				increase = false;
				currState = BEFORE_DOCTYPE_NAME;
			}
			break;
		case BEFORE_DOCTYPE_NAME:
			switch (str[currPosition]) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '\f':
				break;
			case '>':
				returnType = DOCTYPETOK;
				shouldReturn = true;
				break;
			default:
				if (isupper(str[currPosition])) {
					currToken.push_back(tolower(str[currPosition]));
				}
				else {
					currToken.push_back(tolower(str[currPosition]));
				}
			}
			break;
		case DOCTYPE_NAME:
			switch (str[currPosition]) {
			}
			break;
		case BEFORE_ATTRIBUTE_NAME:
			switch (str[currPosition]) {
				//READ: formfeed character
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '\f':
				break;
			case '>':
			case '/':
				increase = false;
				currState = AFTER_ATTRIBUTE_NAME;
				break;
			case '=':
				exit(EXIT_FAILURE);
				break;
			default:
				currState = ATTRIBUTE_NAME;
				increase = false;
			}
			break;
		case ATTRIBUTE_NAME:
			switch (str[currPosition]) {
			case '\'':
			case '<':
			case '"':
				exit(EXIT_FAILURE);
			case ' ':
			case '\t':
			case '\f':
			case '\r':
			case '\n':
				currState = AFTER_ATTRIBUTE_NAME;
				break;
			case '=':
				currState = BEFORE_ATTRIBUTE_VALUE;
				break;
			default:
				if (isupper(str[currPosition]))
				{
					currToken.push_back(tolower(str[currPosition]));

				}
				else {
					currToken.push_back(str[currPosition]);
				}
			}
			break;
		case AFTER_ATTRIBUTE_NAME:
			switch (str[currPosition]) {
			case '\t':
			case ' ':
			case '\f':
			case '\r':
				break;
			case '=':
				currState = BEFORE_ATTRIBUTE_VALUE;
				break;
			case '>':
				currState = DATA;
				shouldReturn = true;//return current tag
				returnType = TAG;
				break;
			default:
				currState = ATTRIBUTE_NAME;
				increase = false;
			}
			break;
		case BEFORE_ATTRIBUTE_VALUE:
			switch (str[currPosition]) {
			case '\t':
			case ' ':
			case '\f':
			case '\r':
				break;
			case '"':
				currState = ATTRIBUTE_VALUE_DOUBLE_QUOTE;
				break;
			case '\'':
				currState = ATTRIBUTE_VALUE_SINGLE_QUOTE;
				break;
			case '>':
				//TODO: handle later
				shouldReturn = true;//return current tag
				returnType = TAG;
				exit(EXIT_FAILURE);
				break;
			default:
				currState = ATTRIBUTE_VALUE_UNQUOTED;
				increase = false;
			}
			break;
		case ATTRIBUTE_VALUE_DOUBLE_QUOTE:
			switch (str[currPosition]) {
			case '"':
				currState = AFTER_ATTRIBUTE_VALUE_QUOTED;
				break;
			case '&':
				//TODO: later 
				break;
			default:
				currToken.push_back(str[currPosition]);//add to attribute value
			}
			break;
		case ATTRIBUTE_VALUE_SINGLE_QUOTE:
			switch (str[currPosition]) {
			case '\'':
				currState = AFTER_ATTRIBUTE_VALUE_QUOTED;
				break;
			case '&':
				//TODO: later 
				break;
			default:
				currToken.push_back(str[currPosition]);//add to attribute value
			}
			break;
		case ATTRIBUTE_VALUE_UNQUOTED:
			switch (str[currPosition]) {
			case '\t':
			case ' ':
			case '\f':
			case '\r':
				currState = BEFORE_ATTRIBUTE_NAME;
				break;
			case '&':
				//TODO: later
				break;
			case '>':
				shouldReturn = true;//return current  tag 
				returnType = TAG;
				currState = DATA;
				break;
			case '"':
			case '\'':
			case '<':
			case '=':
			case '`':
				exit(EXIT_FAILURE);
				break;
			default:
				currToken.push_back(str[currPosition]);//add to attribute value
			}
			break;
		case AFTER_ATTRIBUTE_VALUE_QUOTED:
			switch (str[currPosition]) {
			case '\t':
			case ' ':
			case '\f':
			case '\r':
				currState = BEFORE_ATTRIBUTE_NAME;
				break;
			case '/':
				currState = SELF_CLOSING_START_TAG;
				break;
			case '>':
				currState = DATA;
				shouldReturn = true;//return current tag
				returnType = TAG;
				break;
			default:
				//ERROR: missing whitespace
				currState = BEFORE_ATTRIBUTE_NAME;
				increase = false;
			}
			break;
		case COMMENT_START:
			switch (str[currPosition]) {
			case '-':
				currState = COMMENT_START_DASH;
				break;
			case '>':
				//ERROR: abrupt closing of comment
				currState = DATA;
				break;
			default:
				increase = false;
				currState = COMMENT;
			}
			break;
		case COMMENT_START_DASH:
			switch (str[currPosition]) {
			case '-':
				currState = COMMENT_END;
				break;
			case '>':
				//ERROR: abrupt closing of comment
				currState = DATA;
				break;
			default:
				currToken.push_back('-');//append to comment token
				increase = false;
				currState = COMMENT;

			}
			break;
		case COMMENT:
			switch (str[currPosition]) {
			case '<':
				currToken.push_back(str[currPosition]);//append to cmt token
				currState = COMMENT_LESS_THAN_SIGN;
				break;
			case '-':
				currState = COMMENT_END_DASH;
				break;
			default:
				currToken.push_back(str[currPosition]);//append to cmt token
			}
			break;
		case COMMENT_LESS_THAN_SIGN:
			switch (str[currPosition]) {
			case '!':
				currToken.push_back(str[currPosition]);//append to cmt token
				currState = COMMENT_LESS_THAN_SIGN_BANG;
				break;
			case '<':
				currToken.push_back(str[currPosition]);//append to cmt token
				break;
			default:
				increase = false;
				currState = COMMENT;
			}
			break;
		case COMMENT_LESS_THAN_SIGN_BANG:
			switch (str[currPosition]) {
			case '-':
				currPosition = COMMENT_LESS_THAN_SIGN_BANG_DASH;
				break;
			default:
				increase = false;
				currState = COMMENT;
			}
			break;
		case COMMENT_LESS_THAN_SIGN_BANG_DASH:
			switch (str[currPosition]) {
			case '-':
				currPosition = COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
				break;
			default:
				increase = false;
				currState = COMMENT;
			}
			break;
		case COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH:
			switch (str[currPosition]) {
			case '>':
				increase = false;
				currState = COMMENT_END;
				break;
			default:
				//ERROR: nested comment
				exit(EXIT_FAILURE);
			}
			break;
		case COMMENT_END_DASH:
			switch (str[currPosition]) {
			case '-':
				currState = COMMENT_END;
				break;
			default:
				currToken.push_back('-');//append to comment token
				increase = false;
				currState = COMMENT;
			}
			break;
		case COMMENT_END:
			switch (str[currPosition]) {
			case '>':
				currState = DATA;
				shouldReturn = true;//return cmt token
				returnType = TOK_COMMENT;

				break;
			case '!':
				currState = COMMENT_END_BANG;
				break;
			case '-':
				currToken.push_back('-');//append to cmt
				break;
			default:
				currToken.append("--");//append to cmt
				increase = false;
				currState = COMMENT;

			}
			break;
		case COMMENT_END_BANG:
			switch (str[currPosition]) {
			case '-':
				currToken.append("--!");//append to cmt
				currState = COMMENT_END_DASH;
				break;
			case '>':
				//ERROR: incorrect closing of comment
				currState = DATA;
				shouldReturn = true;//return cmt
				break;
			default:
				currToken.append("--!");//append to cmt
				increase = false;
				currState = COMMENT;
			}
			break;
		}
		if (increase) {
			currPosition++;
		}
		else {
			increase = true;
		}
		if (shouldReturn) {
			return (Token{ .type = returnType,.token = currToken });
		}
	}
}


/*

*/
//polymorphism
