#include<algorithm>
#include"rendertree.h"
#include"cssparser.h"

#define RENDERTREE_LOG 

#define endl '\n'
using std::cout;
RenderTree::RenderTree() {}

RenderTree::RenderTree(Element* elem, Style* stptr)
	: element{ elem },
	styles{ stptr }
{
}

Window* RenderTree::windowptr = nullptr;
RenderTree::~RenderTree() {
	for (std::vector<RenderTree*>::iterator it = children.begin(); it != children.begin(); it++) {
		delete(*it);
		std::cout << "delete render tree element" << endl;
	}
	delete styles;
}

bool RenderTree::isRenderable(Style* styleptr) {
	if (styleptr->getDisplay() == styles::Display::NONE) {
		return false;
	}
	return true;
}

void RenderTree::createFromDom(Document* document) {
	//assuming document has only one child and which is HTMl
	element = dynamic_cast<Element*>(document->childNodes[0]);
	if (!element) {
		cout << "Document has no child html" << endl;
		exit(EXIT_FAILURE);
	}
	this->styles = new SS::HTMLHtmlStyle{};
	this->addChild(document->childNodes[0]);
}
void RenderTree::addChild(Node* node) {
	for (const auto& currNode : node->childNodes) {
		Element* elptr = nullptr;
		elptr = dynamic_cast<Element*>(currNode);
		if (elptr) {
			Style* styleptr = addDefaultStyleSheets(elptr);
			if (!isRenderable(styleptr)) {
				delete styleptr;
				continue;
			}
			RenderTree* rt = new RenderTree{ elptr,styleptr };
			rt->parent = this;
			if (!this->children.empty()) {
				rt->prevSibling = this->children.back();
			}
			this->children.push_back(rt);
			if (currNode->childNodes.size() == 0) {
				//this->calculateLayout();
			}
			else {
				rt->addChild(currNode);
				//this->calculateLayout();
			}
		}
	}
}

Style* RenderTree::addDefaultStyleSheets(Element* el) {
	Style* styleptr = nullptr;
	switch (getTagNameAsEnum(el->tagName)) {
	case HTML:
		styleptr = new SS::HTMLHtmlStyle{};
		break;
	case HEAD:
		styleptr = new SS::HTMLHeadStyle{};
		break;
	case META:
		styleptr = new SS::HTMLMetaStyle{};
		break;
	case BODY:
		styleptr = new SS::HTMLBodyStyle{};
		break;
	case TITLE:
		styleptr = new SS::HTMLTitleStyle{};
		break;
	case P:
		styleptr = new SS::HTMLPStyle{};
		break;
	case DIV:
		styleptr = new SS::HTMLDivStyle{};
		break;
	case NAV:
		styleptr = new SS::HTMLNavStyle{};
		break;
	case UL:
		styleptr = new SS::HTMLUlStyle{};
		break;
	case H1:
		styleptr = new SS::HTMLH1Style{};
		break;
	case H2:
		styleptr = new SS::HTMLH2Style{};
		break;
	case H3:
		styleptr = new SS::HTMLH3Style{};
		break;
	case H4:
		styleptr = new SS::HTMLH4Style{};
		break;
	case H5:
		styleptr = new SS::HTMLH5Style{};
		break;
	case H6:
		styleptr = new SS::HTMLH6Style{};
		break;
	case SPAN:
		//styleptr = new SS::HTMLStrikeStyle{};
		//break;
	default:
		cout << "TAG NOT IMPLEMENTED" << endl;
		exit(EXIT_FAILURE);
	}
	return styleptr;
	//
	//SDL_Color c=this->getColor();
}

bool isBlockLevelElement(const std::string& tn) {

	//switch (getTagNameAsEnum(this->element->tagName)) {
	switch (getTagNameAsEnum(tn)) {
	case BODY:
	case DIV:
	case ARTICLE:
	case NAV:
	case H1:
	case H2:
	case H3:
	case H4:
	case H5:
	case H6:
	case UL:
	case OL:
	case P:
		return true;

		//block level
		/*
		double marginx = this->styles->mmargin[1] +
			this->styles->mmargin[3];
		double borderx = this->styles->mborder[1] +
			this->styles->mborder[3];
		double paddingx = this->styles->mpadding[1] +
			this->styles->mpadding[3];
		*/
		//x-> parents x
		//y-> if(first child) parents y preceding siblings y+ its height
	}
	return false;
}

void RenderTree::calculateLayout(int w) {
	if (!RenderTree::windowptr) {
		cout << "null winodowptr" << endl;
		exit(EXIT_FAILURE);
	}
	this->rect.w = w;
	for (auto& element : children) {
		element->calculateLayout();
	}
}


/*
this applies to bock level element only

//the width of a element is equal to the innerwidth(content) of its parent
//the height of the element is equal to the sum of the height of its childnodes and its margin padding and border

The layout of a element depends upon its childNodes.
- Iterate through child nodes
- If childNode is a text (find the height and weight required to render it)
- if childNode is an element call calculateLayout [recursive]

-- To calculate the text height and width
- first find the margin,padding and border(top,right and left)
- Then calculate the width of content-box
- calculate height required to render the text
-calculate the height for margin,border and padding (bottom)

no margin case
The x-coordinates of an element is equal to its parent
if first child
the y-coordinates of an element is equal to its parent
else
the y-coordinates will be the y-coordinates of its preceding sibling(Node) + its height

*/
void RenderTree::calculateLayout() {
	//x
	this->rect.x = this->parent->rect.x +
		this->styles->mmargin.left.toPixel() +
		this->parent->styles->mpadding.left.toPixel() +
		this->parent->styles->mborder.left.toPixel()
		;

	//y

	//if prev sibling is text node then 
	if (auto textNodeptr = dynamic_cast<Text*>(this->element->prevSibling)) {
		auto& text = textNodeptr->getText();
		auto [width, height] = RenderTree::windowptr->getFontSize(text, getFontName(), getFontSize());

		int pwidth = this->parent->rect.w -
			this->parent->styles->mpadding.left.toPixel() -
			this->parent->styles->mpadding.right.toPixel() -
			this->parent->styles->mborder.left.toPixel() -
			this->parent->styles->mborder.right.toPixel();
		//the parent of its prevSibling is same a s its parent

		int linecount = static_cast<int>(width / pwidth);
		if (width > pwidth * linecount) {
			linecount++;
		}
		this->rect.y = this->prevSibling->rect.y +
			this->prevSibling->rect.h +
			this->prevSibling->styles->mmargin.bottom.toPixel() +
			linecount * height +
			this->styles->mmargin.top.toPixel();
	}
	else if (this->prevSibling) {
		//if prev sibling is not text node 
		//then it must be element node(if it exist)
		//then this->prevSibling exists only if this->element->prevSibling was renderable

		//auto currPrev = this->element->prevSibling;
		//while (!this->isRenderable(currPrev->styles)) {
		//	currPrev = currPrev->prevSibling;
		//	if (!currPrev) {
		//		//prev currPrev was first chid
		//		//MUST what if currprev sibling is text node
		//		this->rect.y = this->parent->rect.y +
		//			this->parent->styles->mpadding.top.toPixel() +
		//			this->parent->styles->mborder.top.toPixel();
		//		break;
		//	}
		//}
		this->rect.y = this->prevSibling->rect.y +
			this->prevSibling->rect.h +
			this->prevSibling->styles->mmargin.bottom.toPixel() +
			this->styles->mmargin.top.toPixel();
	}
	else {
		this->rect.y = this->parent->rect.y +
			this->parent->styles->mpadding.top.toPixel() +
			this->parent->styles->mborder.top.toPixel();
	}
	this->rect.y += this->styles->mmargin.top.toPixel();

	//width
	if (isBlockLevelElement(this->element->tagName))
	{
		this->rect.w = this->parent->rect.w -
			this->parent->styles->mpadding.left.toPixel() -
			this->parent->styles->mpadding.right.toPixel() -
			this->parent->styles->mborder.left.toPixel() -
			this->parent->styles->mborder.right.toPixel();
		this->rect.w -= this->styles->mmargin.left.toPixel();
		this->rect.w -= this->styles->mmargin.right.toPixel();
	}
	else;

	auto isTextNode = [](Node* node) {
		if (dynamic_cast<Text*>(node))
			return true;
		else
			return false;
	};

	for (auto& element : children) {
		element->calculateLayout();
	}

	//height
	for (auto node : this->element->childNodes) {
		//iterate through child text nodes
		auto textNodeptr = dynamic_cast<Text*>(node);
		if (textNodeptr) {
			//curr Node is textNode
			auto& text = textNodeptr->getText();
			auto [width, height] = RenderTree::windowptr->getFontSize(text, getFontName(), getFontSize());
			int pwidth = this->rect.w -
				this->styles->mpadding.left.toPixel() -
				this->styles->mpadding.right.toPixel() -
				this->styles->mborder.left.toPixel() -
				this->styles->mborder.right.toPixel();

			int linecount = static_cast<int>(width / pwidth);
			if (width > pwidth * linecount) {
				linecount++;
			}
			this->rect.h += linecount * height;
		}
	}

	for (auto rt : children) {
		//iterate through child element nodes
		this->rect.h += rt->rect.h +
			rt->styles->mmargin.top.toPixel() +
			rt->styles->mmargin.bottom.toPixel();
	}

	this->rect.h += this->styles->mpadding.top.toPixel() + this->styles->mpadding.bottom.toPixel();
	this->rect.h += this->styles->mborder.top.toPixel() + this->styles->mborder.bottom.borderWidth.toPixel();


#ifdef RENDERTREE_LOG
	cout << this->element->tagName
		<< "   ";
	cout << endl;
	if (this->element->attributes.size() != 0) {
		cout << this->element->attributes[0].getName() << " : " << this->element->attributes[0].getValue() << endl;
	}
	cout << "(x,y)= " << this->rect.x << "," << this->rect.y << ")" << endl;
	cout << "width: " << this->rect.w << " height: " << rect.h << endl << endl;
#endif
}

void RenderTree::setStatic(Window* window) {
	windowptr = window;
}

void RenderTree::addRootStyle() {
	this->styles->mbackgroundColor = { 255,255,255,255 };
	this->styles->mcolor = { 0,0,0,255 };
	this->styles->mfontSize = 16;
	this->styles->mfontName = "Roboto";
}

static bool operator==(const SDL_Color& c1, const SDL_Color& c2) {
	if (c1.r != c2.r)return false;
	if (c1.g != c2.g)return false;
	if (c1.b != c2.b)return false;
	if (c1.a != c2.a)return false;
	return true;
}

SDL_Color RenderTree::getBgColor() const {
	if (!(this->styles->mbackgroundColor == SDL_Color{ 0,0,0,0 } || this->styles->mbackgroundColor == SDL_Color{ 205,205,205,205 })) {
		return this->styles->mbackgroundColor;
	}
	auto currtree = this->parent;
	while (currtree) {
		if (!(currtree->styles->mbackgroundColor == SDL_Color{ 0,0,0,0 } || currtree->styles->mbackgroundColor == SDL_Color{ 205,205,205,205 })) {
			return currtree->styles->mbackgroundColor;
		}
		currtree = currtree->parent;
	}
	cout << "This element doesnot have specified bg color";
	exit(EXIT_FAILURE);
}

SDL_Color RenderTree::getColor() const {
	/*
	it the elements color is equal to 0,0,0,0 or 205,205,205,205
	the element has no color
	*/

	if (!(this->styles->mcolor == SDL_Color{ 0,0,0,0 } || this->styles->mcolor == SDL_Color{ 205,205,205,205 })) {
		//MUST MUST remove : temporary
		return this->styles->mcolor;
	}
	auto currtree = this->parent;
	while (currtree) {
		if (!(currtree->styles->mcolor == SDL_Color{ 0,0,0,0 } || currtree->styles->mcolor == SDL_Color{ 205,205,205,205 })) {
			return currtree->styles->mcolor;
		}
		currtree = currtree->parent;
	}
	cout << "This element doesnot have specified bg color";
	exit(EXIT_FAILURE);
}

int RenderTree::getFontSize() const {
	int fontsize = this->styles->mfontSize.toPixel();
	if (fontsize != 0)
	{
		return fontsize;
	}
	auto currtree = this->parent;
	while (currtree) {
		fontsize = currtree->styles->mfontSize.toPixel();
		if (fontsize != 0)
		{
			return fontsize;
		}
		currtree = currtree->parent;
	}
	cout << "This element doesnot have specified font size";
	exit(EXIT_FAILURE);
}

std::string RenderTree::getFontName() const {
	auto fontname = this->styles->mfontName;
	if (!fontname.empty())
	{
		return fontname;
	}
	auto currtree = this->parent;
	while (currtree) {
		fontname = currtree->styles->mfontName;
		if (!fontname.empty())
		{
			return fontname;
		}
		currtree = currtree->parent;
	}
	cout << "This element doesnot have specified font ";
	exit(EXIT_FAILURE);
}
void RenderTree::addStyle(const CssParser& css) {
	//for every css property in find the corresponding element and override styles
	for (auto& cssrule : css.cssRules) {
		int c = this->applyCss(cssrule);
		//cout << c << "  total Match" << endl << endl;
	}
}

int RenderTree::applyCss(const CSSRule& rule) {
	int count{};
	for (auto child : this->children) {
		//iterate thorugh child first
		count += child->applyCss(rule);
	}

	struct Selector selector = rule.first;
	std::string selString = selector.type == SelectorType::ID ? "id" : "class";

	bool isHover = selector.name.ends_with("hover");
	if (isHover)
		selector.name.erase(selector.name.length() - std::string{ ":hover" }.length());

	for (auto attribute : this->element->attributes) {
		if (attribute.getName() == selString && attribute.getValue() == selector.name) {
			if (isHover) {
				/* if pseudo class then add hover action to effect when hovered*/
				for (auto propValue : rule.second) {
					this->addHoverAction(propValue);
				}
			}
			else {
				for (auto propValue : rule.second) {
					this->applyStyle(propValue);
				}
			}
		}
	}
	return count;
}
void RenderTree::addHoverAction(Declaration& style) {
	/*Effect that can be use on hover
	- color -backgroundColor */

	//cout << "hover style" << style.value << endl;

	switch (style.property) {
	case CSSProperty::COLOR:
		this->hoverStyle.mcolor.first = true;
		this->hoverStyle.mcolor.second = styles::parseColor(style.value);
		break;
	case CSSProperty::BACKGROUND_COLOR:
		this->hoverStyle.mbackgroundColor.first = true;
		this->hoverStyle.mbackgroundColor.second = styles::parseColor(style.value);
		break;
	}


};

void RenderTree::applyStyle(Declaration& style) {
	using namespace styles;
	switch (style.property)
	{
	case CSSProperty::COLOR:
		this->styles->mcolor = styles::parseColor(style.value);
		break;
	case CSSProperty::MARGIN:
		this->styles->mmargin = parseLengthList(style.value);
		break;
	case CSSProperty::MARGIN_TOP:
		this->styles->mmargin.top = parseLength(style.value);
		break;
	case CSSProperty::MARGIN_RIGHT:
		this->styles->mmargin.right = parseLength(style.value);
		break;
	case CSSProperty::MARGIN_BOTTOM:
		this->styles->mmargin.bottom = parseLength(style.value);
		break;
	case CSSProperty::MARGIN_LEFT:
		this->styles->mmargin.left = parseLength(style.value);
		break;
	case CSSProperty::PADDING:
		this->styles->mpadding = parseLengthList(style.value);
		break;
	case CSSProperty::PADDING_TOP:
		this->styles->mpadding.top = parseLength(style.value);
		break;
	case CSSProperty::PADDING_RIGHT:
		this->styles->mpadding.right = parseLength(style.value);
		break;
	case CSSProperty::PADDING_BOTTOM:
		this->styles->mpadding.bottom = parseLength(style.value);
		break;
	case CSSProperty::PADDING_LEFT:
		this->styles->mpadding.left = parseLength(style.value);
		break;
	case CSSProperty::BORDER:
	{
		styles::Border border = styles::parseBorder(style.value);
		this->styles->mborder = { border, border, border, border };
		break;
	}
	case CSSProperty::BORDER_TOP:
		this->styles->mborder.top = styles::parseBorder(style.value);
		break;
	case CSSProperty::BORDER_RIGHT:
		this->styles->mborder.right = styles::parseBorder(style.value);
		break;
	case CSSProperty::BORDER_BOTTOM:
		this->styles->mborder.bottom = styles::parseBorder(style.value);
		break;
	case CSSProperty::BORDER_LEFT:
		this->styles->mborder.left = styles::parseBorder(style.value);
		break;
	case CSSProperty::BACKGROUND_COLOR:
		this->styles->mbackgroundColor = styles::parseColor(style.value);
		break;
	case CSSProperty::FONT:
		this->styles->mfontName = style.value;
		break;
	case CSSProperty::FONT_SIZE:
		this->styles->mfontSize = styles::parseLength(style.value);
		break;
	case CSSProperty::FONT_STYLE:
		//this->styles->mfontStyle = styles::parseFontStyle(style.value);
		break;
	case CSSProperty::WIDTH:
		break;
	case CSSProperty::UNKNOWN:
		break;
	default:
		break;
	}
}

//TODO: add support for font size
