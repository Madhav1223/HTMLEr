#pragma once
#include"node.h"
#include"states.h"
#include"defaultstyle.h"
#include"window.h"
#include"csstypes.h"

//#include "cssparser.h"

class CssParser;

class RenderTree {
private:
	static Window* windowptr;
	Element* element = nullptr;
	Style* styles = nullptr;
	DynamicStyle hoverStyle;
	SDL_Rect rect;
	RenderTree* parent{ nullptr };
	RenderTree* prevSibling{ nullptr };
	std::vector<RenderTree*> children;
	int applyCss(const CSSRule& rule);
	void applyStyle(Declaration& style);
	void calculateLayout();
	void calculateBox();
public:
	void addRootStyle();
	static void setStatic(Window* window);
	SDL_Color getBgColor()const;
	SDL_Color getColor()const;

	int getFontSize() const;
	std::string getFontName() const;
	friend class Window;
	RenderTree();
	RenderTree(Element* elem, Style* stptr);
	~RenderTree();
	bool isRenderable(Style* styleptr);
	void createFromDom(Document* document);
	Style* addDefaultStyleSheets(Element* el);
	void addChild(Node* node);
	void calculateLayout(int);
	void addStyle(const CssParser& css);
	//if html has color let the color be that color
	//if not let the color of html be white
	//parse till the html element until it gets color
	//ifnot found: not possible html shoulud have color white by defualt
	void addHoverAction( Declaration& style);

};

