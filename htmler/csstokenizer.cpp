#include"csstokenizer.h"

#define WHITESPACE ' ':\
case '\n':\
case '\t':\
case '\r':\
case '\f'


void CssTokenizer::reset() {
	reconsume = false;
	returnToken = false;
	currToken = "";
}
CssTokenizer::CssTokenizer()
	:returnType{ ReturnType::SELECTOR },//default
	tokenState{ CssTokenState::INITIAL },
	reconsume{ false },
	returnToken{ false }
{
}

static void ignoreComment(const std::string& str, int& currPosition) {
	if (str[currPosition] != '*') {
		cout << "Invalid comment string" << endl;
		exit(EXIT_FAILURE);
	}
	while (currPosition++) {
		if (currPosition >= str.length()) {
			cout << "Invalid comment string" << endl;
			exit(EXIT_FAILURE);
		}
		if (str[currPosition] == '*') {
			if (str[++currPosition] == '/') {
				return;
			}
		}
	}
}
CssToken CssTokenizer::getNextToken(const std::string& str, int& currPosition) {
	reset();

	while (currPosition < str.length()) {
		char current = str[currPosition];
		switch (tokenState)
		{
		case CssTokenState::INITIAL:
			switch (str[currPosition]) {
			case WHITESPACE:
				//ignore allwhitespace
				break;
			case '/':
				ignoreComment(str, ++currPosition);
				break;
			case '.':
			case '#':
				currToken.push_back(str[currPosition]);
				tokenState = CssTokenState::BEFORE_SELECTOR_NAME;
				break;
			default:
				exit(EXIT_FAILURE);
			}
			break;
		case CssTokenState::BEFORE_SELECTOR_NAME:
			if (std::isdigit(str[currPosition])) {
				cout << "Selectors Name cannot start with number" << endl;
				exit(EXIT_FAILURE);
			}
			else if (str[currPosition] == '_') {
				if (str[currPosition + 1] == '_' || std::isdigit(str[currPosition])) {
					cout << "Selector name cannot start with a hypern followed by a number or hyphen." << endl;
					exit(EXIT_FAILURE);
				}

			}
			//if not error in above
			reconsume = true;
			tokenState = CssTokenState::SELECTOR_NAME;
			break;
		case CssTokenState::SELECTOR_NAME:
			switch (str[currPosition]) {
			case WHITESPACE:
				tokenState = CssTokenState::AFTER_SELECTOR_NAME;
				break;
			case '{':
				returnType = ReturnType::SELECTOR;
				returnToken = true;
				tokenState = CssTokenState::BEFORE_DECLARATION;
				break;
			default:
				if (
					std::isalpha(str[currPosition]) ||
					std::isdigit(str[currPosition]) ||
					str[currPosition] == '_' ||
					str[currPosition] == '-'||
					str[currPosition]==':'
					) {
					currToken.push_back(str[currPosition]);
				}
				else {
					cout << "Illegal Character: '" << str[currPosition] << "'" << endl;
					exit(EXIT_FAILURE);
				}
			}
			break;
		case CssTokenState::AFTER_SELECTOR_NAME:
			switch (str[currPosition]) {
			case WHITESPACE:
				//ignore whitespaces
				break;
			case '/':
				ignoreComment(str, ++currPosition);
				break;
			case '{':
				tokenState = CssTokenState::BEFORE_DECLARATION;
				returnType = ReturnType::SELECTOR;
				returnToken = true;
				break;
			default:
				cout << "Illegal Character: '" << str[currPosition] << "'" << endl;
				exit(EXIT_FAILURE);
			}
			break;
		case CssTokenState::BEFORE_DECLARATION:
			switch (str[currPosition]) {
			case WHITESPACE:
				//ignore all preceding whitespace
				break;
			case '/':
				ignoreComment(str, ++currPosition);
				break;
			case ';':
				//ignore empty semicolon
				break;
			case '}':
				tokenState = CssTokenState::INITIAL;
				break;
			default:
				reconsume = true;
				tokenState = CssTokenState::DECLARATION;
				break;
			}
			break;
		case CssTokenState::DECLARATION:
			switch (str[currPosition]) {
			case WHITESPACE:
				if (!currToken.empty() && currToken.back() != ' ') {
					currToken.push_back(' ');
				}
				break;
			case ';':
				returnType = ReturnType::DECLARATION;
				returnToken = true;
				tokenState = CssTokenState::BEFORE_DECLARATION;
				break;
			case '}':
				cout << "Illegal Character: '" << str[currPosition] << "'" << endl;
				exit(EXIT_FAILURE);
			default:
				currToken.push_back(str[currPosition]);
			}
			break;
		}
		if (reconsume == true) {
			reconsume = false;
		}
		else {
			currPosition++;
		}
		if (returnToken) {
			return CssToken{ returnType,currToken };
		}
	}
	if (currToken.empty()) {
		return CssToken(ReturnType::END_OF_FILE, "");
	}
}