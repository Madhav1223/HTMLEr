#include<iostream>
#include "parser.h"
#include "tokenizer.h"
#include"node.h"
#include"elements.h"
using std::cout;
#define endl '\n'

Parser::Parser(Document* doc)
	:parseState{ BEFORE_HTML },
	mdocument{ doc },
	//:parseState{ INITIAL },
	currPosition{}
{
}
std::ostream& operator<<(std::ostream& out, struct Token token) {
	out << getTokenType(token.type) << ": " << token.token << " ";
	return out;
}

//either element or cildText Node
Element* Parser::createNewHtmlElement(TagName tn) {
	Element* el = nullptr;
	switch (tn) {
	case HTML:
		el = new HTMLHtmlElement{};
		break;
	case HEAD:
		el = new HTMLHeadElement{};
		break;
	case BODY:
		el = new HTMLBodyElement{};
		break;
	case TITLE:
		el = new HTMLTitleElement{};
		break;
	case P:
		el = new HTMLParagraphElement{};
		break;
	case DIV:
		el = new HTMLDivElement{};
		break;
	case H1:
		el = new HTMLHeadingElement{ "h1" };
		break;
	case H2:
		el = new HTMLHeadingElement{ "h2" };
		break;
	case H3:
		el = new HTMLHeadingElement{ "h3" };
		break;
	case H4:
		el = new HTMLHeadingElement{ "h4" };
		break;
	case H5:
		el = new HTMLHeadingElement{ "h5" };
		break;
	case H6:
		el = new HTMLHeadingElement{ "h6" };
		break;
	case SPAN:
	default:
		el = new HTMLElement{ "el" };
		break;
	}
	return el;
}
void Parser::create_element_for_token(const TagTokenizer& tt) {
	Element* el = createNewHtmlElement(tt.getTagName());
	el->setAttributes(tt.getAttributes());
	//cout << "create " << endl;
	if (mstack.empty()) {
		mdocument->appendChild(el);
		mstack.push(el);//this must be a html element
		return;
	}
	el->setParentNode(mstack.top());
	mstack.top()->appendChild(el);
	//leave attribute for now
	mstack.push(el);
}

void Parser::create_element_for_token(const std::string& tn) {
	Element* el = createNewHtmlElement(getTagNameAsEnum(tn));
	//cout << "create " << endl;
	if (mstack.empty()) {
		mdocument->appendChild(el);
		mstack.push(el);//this must be a html element
		return;
	}
	mstack.top()->appendChild(el);
	//leave attribute for now
	mstack.push(el);
}

void Parser::insert_character(const std::string& ch) {
	Text* te = new Text(ch);
	mstack.top()->appendChild(te);
	//if current node be document return
	//if text node in previous node append to text node
}
void Parser::generic_rcdata_parse(const std::string& tn) {
	Element el{ tn };
	originalParseState = parseState;
	//switch tokenizer to rcdata state?
	parseState = TEXT;
	//leave attribute for now
	//mstack.push(el);
}

void Parser::parse(const std::string& str) {
	Tokenizer tokenizer;
	struct Token token;
	bool reprocessToken = false;
	TagTokenizer tt;

	while (currPosition < str.length()) {
		if (!reprocessToken) {
			token = tokenizer.getNextToken(str, currPosition);
			if (token.type == TAG || token.type == END_TAG) {
				//cout << token.token << endl;
			}
		}
		else {
			reprocessToken = false;
		}
		if (token.type == TAG) {
			//mstack.push(getTagNameAsEnum(token.token));
			tt = token.token;
		}
		switch (parseState) {
		case INITIAL:
			switch (token.type) {
			case DOCTYPE:
				parseState = BEFORE_HTML;
				break;//later
			case COMMENT:
				////document.appendChild(Comment token.token);//document cant have more than one child so ignore for now
				break;
			default:
				parseState = BEFORE_HTML;
				reprocessToken = true;
				//exit(EXIT_FAILURE);//TODO:
			}
			break;
		case BEFORE_HTML:
			switch (token.type) {
			case DOCTYPE:
				//ignore the token
				break;
			case COMMENT:
				//ignore 
				break;
			case TAG:
				switch (tt.getTagName()) {
				case HTML:
				{
					create_element_for_token(tt);
					parseState = BEFORE_HEAD;
					break;
				}
				default:
					exit(EXIT_FAILURE);
				}
				break;
			case END_TAG:
				TagName tempTagName;
				tempTagName = getTagNameAsEnum(token.token);//for no compiler error
				if (tempTagName == HTML || tempTagName == HEAD || tempTagName == BODY
					|| tempTagName == BR) {
					//will fallthrough to default;
										//NOTE: the case END_TAG must always be just above default case
				}
				else {
					//ignore the token
					break;
				}
				[[fallthrough]];
			default:
				create_element_for_token("html");
				parseState = BEFORE_HEAD;
				reprocessToken = true;
				break;

			}
			break;
		case BEFORE_HEAD:
			switch (token.type) {
			case DOCTYPE:
				//ignore
				break;
			case COMMENT:
				//ignore
				break;
			case TAG:
				switch (tt.getTagName()) {
				case HTML:
					//TODO: 
					//Process the token using the rules for the "in body" insertion mode.
					break;
				case HEAD:
					//set head pointer 
					create_element_for_token(tt);
					//*headptr=
					parseState = IN_HEAD;

				}
				break;
			case END_TAG:
				TagName tempTagName;
				tempTagName = getTagNameAsEnum(token.token);
				if (tempTagName == HTML || tempTagName == HEAD || tempTagName == BODY
					|| tempTagName == BR) {
					//will fallthrough to default
					//NOTE: the case END_TAG must always be just above default case
				}
				else {
					//ignore 
					break;
				}
				[[fallthrough]];
			default:
				create_element_for_token("head");
				//set head element pointer
				parseState = IN_HEAD;
				reprocessToken = true;
			}
			break;
		case IN_HEAD:
			switch (token.type) {
			case COMMENT:
				//ignore 
				break;
			case DOCTYPE:
				//ignore
				break;
			case TAG:
				switch (tt.getTagName()) {
				case HTML:
					//TODO:
					// Process the token using the rules for the "in body" insertion mode.
					break;
				case BASE:
					//case BASEFONT:
					//case BGSOUND:
				case LINK:
				case STYLE:
				case SCRIPT:
					exit(EXIT_FAILURE);
					break;
					cout << "UNECOGNIZED TAG NAME" << endl;
					exit(EXIT_FAILURE);
					//idk ept
					//Insert an HTML element for the token. Immediately pop the current node off the stack of open elements.
					break;
				case META:
					//TODO: 
					//ignore for now
					//this->create_element_for_token(tt);
					//mstack.pop();//self closing tag
					break;
				case TITLE:
					this->create_element_for_token(tt);
					originalParseState = parseState;
					parseState = TEXT;
					//rcdata parsing
					break;
				case HEAD:
					//ignore: parse error
					break;
				}
				break;
			case END_TAG:
				TagName tempTagName;
				tempTagName = getTagNameAsEnum(token.token);
				if (tempTagName == HEAD) {
					parseState = AFTER_HEAD;
					mstack.pop();//TODO: check for head to  be the last element
					break;
				}
				else if (tempTagName == BODY || tempTagName == HTML || tempTagName == BR)
				{
					//will fallthrough to default
					//NOTE: the case END_TAG must always be just above default case
				}
				else {
					//ignore : parse error
					break;
				}
			default:
				mstack.pop();
				parseState = AFTER_HEAD;
				reprocessToken = false;
			}
			break;
		case AFTER_HEAD:
			switch (token.type)
			{
			case COMMENT:
				//ignore
				break;
			case DOCTYPE:
				//ignore: parse error
				break;
			case TAG:
				switch (tt.getTagName()) {
				case HEAD:
					//ignore: parse error
					break;
				case HTML:
					//TODO:
					// Process the token using the rules for the "in body" insertion mode.
					break;
				case BODY:
					this->create_element_for_token(tt);
					parseState = IN_BODY;
					break;
				default:
					cout << "UNECOGNIZED TAG NAME" << endl;
					exit(EXIT_FAILURE);
				}
				break;
			case END_TAG:
				TagName tempTagName;
				tempTagName = getTagNameAsEnum(token.token);
				if (tempTagName == BODY || tempTagName == HTML || tempTagName == BR)
				{
					//will fallthrough to default
					//NOTE: the case END_TAG must always be just above default case
				}
				else {
					//ignore : parse error
					break;
				}
				[[fallthrough]];
			default:
				this->create_element_for_token("body");
				parseState = IN_BODY;
				reprocessToken = true;
				break;
			}
			break;
		case IN_BODY:
			switch (token.type)
			{
			case CHARACTER:
				//Reconstruct the active formatting elements, if any.
				insert_character(token.token);
				break;
			case COMMENT:
			case DOCTYPETOK:
				//ignore
				break;
			case TAG:
				TagName tn;
				switch (tn = tt.getTagName()) {
				case HTML:
				case BODY:
					//ignore : parser erro #additional
					break;
				case EOF:
					cout << "UNEXPECTED END OF FILE" << endl;
					exit(EXIT_FAILURE);
				case DIV:
				case P:
				case NAV:
				case ARTICLE:
					//case ADDRESS: case ASIDE: case BLOCKQUOTE: case CENTER: case DETAILS: case DIALOG: case DIR: case DL: case FIELDSET: case FIGCAPTION: case FIGURE: case FOOTER: case HEADER: case HGROUP: case MAIN: case MENU: case SECTION: case SUMMARY: 
					if (mstack.top()->getTagName() == "p") {
						mstack.pop();
					}
					this->create_element_for_token(tt);
					//insert an html element
					break;
				case H1:
				case H2:
				case H3:
				case H4:
				case H5:
				case H6:
					if (mstack.top()->getTagName() == "p") {
						mstack.pop();
					}
					switch (getTagNameAsEnum(mstack.top()->getTagName())) {
					case H1: case H2: case H3: case H4: case H5: case H6:
						//parse error
						mstack.pop();
					}
					this->create_element_for_token(tt);
					break;
				case BUTTON:
					//Reconstruct the active formatting elements, if any.
					this->create_element_for_token(tt);
					break;
				case A:
					cout << "A IMPLEMENTATION WOULD BE MEANINGLESS" << endl;
					exit(EXIT_FAILURE);
					break;
				case LI:
				case UL:
				case OL:
					//no time to implement
					cout << "UNIMPLEMENTED TAG" << endl;
					exit(EXIT_FAILURE);
					break;
				default:
					//TODO: exit for unhandled tags
					//TODO: Reconstruct the active formatting elements, if any.
					//TODO: handle other start tags
					//create_element_for_token(tt);//ordinary elements
					cout << "UNKNOWN START TAG" << endl;
					exit(EXIT_FAILURE);
				}
				break;

				//TODO:macros
				//case FORMATTING ELEMENTS:
				//exit 

			case END_TAG:
			{
				//todo: handle later with all cojnjditions 
				TagName currOpenTag = getTagNameAsEnum(mstack.top()->getTagName());
				TagName currEndTag = getTagNameAsEnum(token.token);
				switch (currEndTag) {
				case BODY:
					if (currOpenTag != BODY) {
						cout << "UNCLOSEED TAG IN BODY" << endl;
						exit(EXIT_FAILURE);
					}
					mstack.pop();
					//TODO: additional
					parseState = AFTER_BODY;
					break;
				case HTML:
					cout << "NO END TAG FOR BODY" << endl;
					exit(EXIT_FAILURE);
					break;
				case H1: case H2: case H3: case H4: case H5: case H6:
				case DIV:
				case P:
				case NAV:
				case ARTICLE:
				case SPAN://MUST: this is not the way to handle span
					if (currEndTag = currOpenTag) {
						mstack.pop();
						break;
					}
					else {
						//strict
						cout << "UNCLOSED TAGS" << endl;
						exit(EXIT_FAILURE);
					}
				default:
					cout << "UNKNOWN END TAGS" << endl;
					exit(EXIT_FAILURE);
				}
			}//case end brace
			break;
			}
			break;
		case TEXT:
			switch (token.type) {
			case CHARACTER:
				insert_character(token.token);//todo
				break;
			case END_TAG:
				if (getTagNameAsEnum(mstack.top()->getTagName()) != getTagNameAsEnum(token.token)) {
					//strict
					cout << "UNEXPECTED END TOOKEN" << endl;
					exit(EXIT_FAILURE);
				}
				mstack.pop();
				parseState = originalParseState;
				break;
			case EOF:
				//parse error
				mstack.pop();
				reprocessToken = true;
				break;
			}
			break;
		}
	}
}

void insertHtmlElement() {
	int adjusted_insertion_location = 1;//=current Node
	int element;//create elemetn
	//push element to stack

}
void Parser::printStackOfOpenElements() {
	while (!mstack.empty()) {
		//cout << getTagName(mstack.top()) << endl;
		mstack.pop();
	}
}


/*
case ADDRESS:
case ARTICLE:
case ASIDE:
case BLOCKQUote:
case CENTER:
case DETAILS:
case DIALOG:
case DIR:
case DIV:
case DL:
case FIELDSEt:
case FIGCAPTion:
case FIGURE:
case FOOTER:
case HEADER:
case HGROUP:
case MAIN:
case MENU:
case NAV:
case OL:
case P:
case SECTION:
case SUMMARY:
case UL:
*/

/*
KNOWINGLY NOT IMPLEMENTED
A
UL
LI
OL
FORM
all form related elements
FORMATTING ELEMENTS
HR - might work on it




*/
