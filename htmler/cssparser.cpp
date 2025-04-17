#include"cssparser.h"

static inline std::string getPropertyName(CSSProperty prop) {
	switch (prop) {
	case CSSProperty::COLOR:return "color";
	case CSSProperty::MARGIN:return "margin";
	case CSSProperty::MARGIN_TOP:return "margin_top";
	case CSSProperty::MARGIN_RIGHT:return "margin_right";
	case CSSProperty::MARGIN_BOTTOM:return "margin_bottom";
	case CSSProperty::MARGIN_LEFT:return "margin_left";
	case CSSProperty::BORDER:return "border";
	case CSSProperty::BORDER_TOP:return "border_top";
	case CSSProperty::BORDER_RIGHT:return "border_right";
	case CSSProperty::BORDER_BOTTOM:return "border_bottom";
	case CSSProperty::BORDER_LEFT:return "border_left";
	case CSSProperty::PADDING:return "padding";
	case CSSProperty::PADDING_TOP:return "padding_top";
	case CSSProperty::PADDING_RIGHT:return "padding_right";
	case CSSProperty::PADDING_BOTTOM:return "padding_bottom";
	case CSSProperty::PADDING_LEFT:return "padding_left";
	case CSSProperty::WIDTH:return "width";
	case CSSProperty::BACKGROUND_COLOR:return "background-color";
	case CSSProperty::FONT:return "font";
	case CSSProperty::FONT_SIZE:return "font-size";
	case CSSProperty::FONT_STYLE:return "font-style";
	case CSSProperty::UNKNOWN:return "unknownn ";
	}
}

inline CSSProperty getPropertyNameAsEnum(const std::string& prop) {
	if (prop == "margin")return CSSProperty::MARGIN;
	if (prop == "color")return CSSProperty::COLOR;
	if (prop == "margin")return CSSProperty::MARGIN;
	if (prop == "margin-top")return CSSProperty::MARGIN_TOP;
	if (prop == "margin-right")return CSSProperty::MARGIN_RIGHT;
	if (prop == "margin-bottom")return CSSProperty::MARGIN_BOTTOM;
	if (prop == "margin-left")return CSSProperty::MARGIN_LEFT;
	if (prop == "border")return CSSProperty::BORDER;
	if (prop == "border-top")return CSSProperty::BORDER_TOP;
	if (prop == "border-right")return CSSProperty::BORDER_RIGHT;
	if (prop == "border-bottom")return CSSProperty::BORDER_BOTTOM;
	if (prop == "border-left")return CSSProperty::BORDER_LEFT;
	if (prop == "padding")return CSSProperty::PADDING;
	if (prop == "padding-top")return CSSProperty::PADDING_TOP;
	if (prop == "padding-right")return CSSProperty::PADDING_RIGHT;
	if (prop == "padding-bottom")return CSSProperty::PADDING_BOTTOM;
	if (prop == "padding-left")return CSSProperty::PADDING_LEFT;
	if (prop == "width")return CSSProperty::WIDTH;
	if (prop == "background-color")return CSSProperty::BACKGROUND_COLOR;
	if (prop == "font")return CSSProperty::FONT;
	if (prop == "font-style")return CSSProperty::FONT_STYLE;
	if (prop == "font-size")return CSSProperty::FONT_SIZE;
	else return CSSProperty::UNKNOWN;
}

inline void CssParser::ignoreWhiteSpace(const std::string& str, int& pos) {
	int temp{};
	while (str[pos] == ' ') {
		if (++temp > 1) {
			cout << "Multiple whitespace in propvaluepair" << endl;
		}
		if (++pos >= str.size()) {
			cout << "Illegal property Value Pair";
			exit(EXIT_FAILURE);
		}
	}
}

CssParser::CssParser()
	:currPosition{ 0 }
{}

void CssParser::parse(const std::string& str) {
	CssTokenizer tokenizer;
	while (currPosition < str.length()) {
		CssToken token = tokenizer.getNextToken(str, currPosition);
		if (token.first == ReturnType::SELECTOR) {
			if (token.second.empty()) {
				cout << "Token cannot be empty" << endl;
				exit(EXIT_FAILURE);
			}
			if (token.second[0] == '.') {
				cssRules.push_back({ {SelectorType::CLASS,token.second.erase(0,1)},{} });
			}
			else if (token.second[0] == '#') {
				cssRules.push_back({ {SelectorType::ID,token.second.erase(0,1)},{} });
			}
			else {
				cout << "Unknow selector" << endl;
				exit(EXIT_FAILURE);

			}
			//cout << "Selector: " << token.second << endl;
		}
		else if (token.first == ReturnType::DECLARATION) {
			Declaration declaration = this->parsePropValue(token.second);
			this->cssRules.back().second.push_back(declaration);
			//cout << "PropValue: Prop: " << getPropertyName(declaration.property) << " Value: " << declaration.value << endl;
		}
		else if (token.first == ReturnType::END_OF_FILE) {
			return;
		}
	}
}

Declaration CssParser::parsePropValue(const std::string& propValue) {
	int pos{};
	std::string prop;
	std::string value;
	while (std::isalpha(propValue[pos]) || propValue[pos] == '-') {
		//extracting property
		prop += propValue[pos];
		if (++pos >= propValue.size()) {
			cout << "Illegal property Value Pair";
			exit(EXIT_FAILURE);
		}
	}
	Declaration decl;
	decl.property = getPropertyNameAsEnum(prop);
	int temp{};
	//ignore whitespace
	while (propValue[pos] == ' ') {
		//should run only once ig
		pos++;
		if (++temp > 1) {
			cout << "Multiple whitespace in propvaluepair" << endl;
		}
	}
	if (!propValue[pos++] == ':') {
		cout << "Illegal property Value Pair";
		exit(EXIT_FAILURE);
	}
	ignoreWhiteSpace(propValue, pos);
	while (pos < propValue.length()) {
		decl.value += propValue[pos++];
	}
	return decl;
}
