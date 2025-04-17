#pragma once
#include<vector>
#include<string>

enum class CSSProperty {
	COLOR,
	MARGIN,
	MARGIN_TOP,
	MARGIN_RIGHT,
	MARGIN_BOTTOM,
	MARGIN_LEFT,
	BORDER,
	BORDER_TOP,
	BORDER_RIGHT,
	BORDER_BOTTOM,
	BORDER_LEFT,
	PADDING,
	PADDING_TOP,
	PADDING_RIGHT,
	PADDING_BOTTOM,
	PADDING_LEFT,
	WIDTH,
	BACKGROUND_COLOR,
	FONT,
	FONT_SIZE,
	FONT_STYLE,
	UNKNOWN
};
enum class SelectorType {
	CLASS,
	ID
};
struct Declaration {
	CSSProperty property;
	std::string value;
};

struct Selector {
	SelectorType type;
	std::string name;
};

typedef std::vector<Declaration> DeclarationList;
typedef  std::pair<Selector, DeclarationList> CSSRule;
