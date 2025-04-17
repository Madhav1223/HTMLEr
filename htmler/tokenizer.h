#pragma once
#include<string>
#include<vector>
#include<string>
#include "states.h"
#include "elements.h"
#include "node.h"

class Tokenizer {
private:
	TokenState currState;
	std::string currToken;
	//int currPosition;
	bool increase;
	bool shouldReturn;
	void reset();
	TokenType returnType;//CHARACTER by default
public:
	Tokenizer();
	struct Token getNextToken(const std::string& str, int& currPosition);

};

enum class TTstate {
	TAG_NAME,
	BEFORE_ATTRIBUTE_NAME,
	ATTRIBUTE_NAME,
	BEFORE_ATTRIBUTE_VALUE,
	AFTER_ATTRIBUTE_NAME,
	ATTRIBUTE_VALUE_DOUBLE_QUOTE,
	ATTRIBUTE_VALUE_SINGLE_QUOTE,
	ATTRIBUTE_VALUE_UNQUOTED,
	AFTER_ATTRIBUTE_VALUE_QUOTED,
	SELF_CLOSING_START_TAG
};
class TagTokenizer {
private:
	TagName tagName;
	std::vector<Attribute> attributes;
	bool selfClosingTag{ false };
public:
	TagTokenizer() = default;
	TagTokenizer(const std::string& str) {
		int currPosition = 0;
		bool reconsume = false;
		std::string currToken;
		TTstate currState = TTstate::TAG_NAME;
		/*head class="fuck you" id="hello"*/
		while (currPosition != str.length()) {
			switch (currState) {
			case TTstate::TAG_NAME:
				switch (str[currPosition]) {
				case '\t':
				case '\n':
				case '\f':
				case ' ':
					tagName = getTagNameAsEnum(currToken);
					currToken = "";
					currState = TTstate::BEFORE_ATTRIBUTE_NAME;
					break;
				case '>':
					//shouldReturn = true;
					tagName = getTagNameAsEnum(currToken);
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
					break;
				}
				break;
			case TTstate::BEFORE_ATTRIBUTE_NAME:
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
					reconsume = true;
					currState = TTstate::AFTER_ATTRIBUTE_NAME;
					break;
				case '=':
					exit(EXIT_FAILURE);
					break;
				default:
					currState = TTstate::ATTRIBUTE_NAME;
					reconsume = true;
				}
				break;
			case TTstate::ATTRIBUTE_NAME:
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
				case '/':
				case '>':
					reconsume = true;
					currState = TTstate::AFTER_ATTRIBUTE_NAME;
					attributes.push_back(Attribute{ currToken,"true" });
					//attributes[attributes.size()-1 ].setValue("true");
					break;

				case '=':
					attributes.push_back(Attribute{ currToken,"" });
					currToken = "";
					currState = TTstate::BEFORE_ATTRIBUTE_VALUE;
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
			case TTstate::BEFORE_ATTRIBUTE_VALUE:
				switch (str[currPosition]) {
				case '\t':
				case ' ':
				case '\f':
				case '\r':
					break;
				case '"':
					currState = TTstate::ATTRIBUTE_VALUE_DOUBLE_QUOTE;
					break;
				case '\'':
					currState = TTstate::ATTRIBUTE_VALUE_SINGLE_QUOTE;
					break;
				case '>':
					//TODO: handle later
					//shouldReturn = true;//return current tag
					//returnType = TAG;
					if (currToken.length() == 0) {
						attributes[attributes.size()-1].setValue("true");
					}
					else {
						attributes[attributes.size()-1 ].setValue(currToken);
					}
					//exit(EXIT_FAILURE);
					break;
				default:
					currState = TTstate::ATTRIBUTE_VALUE_UNQUOTED;
					reconsume = true;
				}
				break;
			case TTstate::AFTER_ATTRIBUTE_NAME:
				switch (str[currPosition]) {
				case '\t':
				case ' ':
				case '\f':
				case '\r':
					break;
				case '=':
					attributes.push_back(Attribute{ currToken,"" });
					currState = TTstate::BEFORE_ATTRIBUTE_VALUE;
					break;
				case '/':
					attributes.push_back(Attribute{ currToken,"true " });
					currState = TTstate::SELF_CLOSING_START_TAG;
					break;
				case '>':
					//currState = DATA;
					//attributes[attributes.size()-1 ].setValue("true");
					//shouldReturn = true;//return current tag
					//returnType = TAG;
					break;
				default:
					currState = TTstate::ATTRIBUTE_NAME;
					reconsume = true;
				}
				break;
			case TTstate::ATTRIBUTE_VALUE_DOUBLE_QUOTE:
				switch (str[currPosition]) {
				case '"':
					attributes[attributes.size()-1].setValue(currToken);
					currToken = "";
					currState = TTstate::AFTER_ATTRIBUTE_VALUE_QUOTED;
					break;
				case '&':
					//TODO: later 
					break;
				default:
					currToken.push_back(str[currPosition]);//add to attribute value
				}
				break;
			case TTstate::ATTRIBUTE_VALUE_SINGLE_QUOTE:
				switch (str[currPosition]) {
				case '\'':
					attributes[attributes.size()-1].setValue(currToken);
					currToken = "";
					currState = TTstate::AFTER_ATTRIBUTE_VALUE_QUOTED;
					break;
				case '&':
					//TODO: later 
					break;
				default:
					currToken.push_back(str[currPosition]);//add to attribute value
				}
				break;
			case TTstate::ATTRIBUTE_VALUE_UNQUOTED:
				switch (str[currPosition]) {
				case '\t':
				case ' ':
				case '\f':
				case '\r':
					currState = TTstate::BEFORE_ATTRIBUTE_NAME;
					break;
				case '&':
					//TODO: later
					break;
				case '>':
					attributes[attributes.size()].setValue(currToken);
					//shouldReturn = true;//return current  tag 
					//returnType = TAG;
					//currState = DATA;
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
			case TTstate::AFTER_ATTRIBUTE_VALUE_QUOTED:
				switch (str[currPosition]) {
				case '\t':
				case ' ':
				case '\f':
				case '\r':
					currState = TTstate::BEFORE_ATTRIBUTE_NAME;
					break;
				case '/':
					currState = TTstate::SELF_CLOSING_START_TAG;
					break;
				case '>':
					//currState = DATA;
					//shouldReturn = true;//return current tag
					//returnType = TAG;
					break;
				default:
					//ERROR: missing whitespace
					currState = TTstate::BEFORE_ATTRIBUTE_NAME;
					reconsume = true;
				}
				break;
			case TTstate::SELF_CLOSING_START_TAG:
			switch (str[currPosition]) {
			case '>':
				selfClosingTag = true;
				break;
			default:
				exit(EXIT_FAILURE);
			}
			break;

			}
			if (reconsume) {
				reconsume = false;
			}
			else {
				currPosition++;
			}
		}
	}
	TagName getTagName()const { return tagName; }
	const std::vector<Attribute> getAttributes()const { return attributes; }
};
