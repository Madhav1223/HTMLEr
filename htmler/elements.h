#pragma once
#include<string>
#include "node.h"
//refrence URL="https://www.w3.org/TR/1998/REC-DOM-Level-1-19981001/level-one-html.html"
class HTMLElement : public Element {
protected:
	std::string id;
	std::string title;
	std::string lang;
	std::string dir;
	std::string outerText;
public:
	HTMLElement(const std::string& tn) :Element{ tn } {}
};

class HTMLDivElement : public HTMLElement {
public:
	HTMLDivElement() :HTMLElement{ "div" } {}
};

class HTMLSpanElement : public HTMLElement {
public:
	HTMLSpanElement() :HTMLElement{ "span" } {}
};

class HTMLAnchorElement : public HTMLElement {
private:
	std::string taget;
public:
	HTMLAnchorElement() :HTMLElement{ "a" } {}
};

class HTMLParagraphElement : public HTMLElement {
	std::string align;
public:
	HTMLParagraphElement() :HTMLElement{ "p" } {}

};

class HTMLHtmlElement : public HTMLElement {
	std::string version;
public:
	HTMLHtmlElement() :HTMLElement{ "html" } {}
};
class HTMLHeadElement : public HTMLElement {
	std::string profile;
public:
	HTMLHeadElement() :HTMLElement{ "head" } {}
};

class HTMLTitleElement : public HTMLElement {
	std::string text;
public:
	HTMLTitleElement() :HTMLElement{ "title" } {}
};

class HTMLBodyElement : public HTMLElement {
	std::string aLink;
	std::string background;
	std::string bgColor;
	std::string link;
	std::string text;
	std::string vLink;
public:
	HTMLBodyElement() :HTMLElement{ "body" } {}
};

class HTMLHeadingElement : public HTMLElement {
	std::string align;
public:
	HTMLHeadingElement(const std::string& tagName) :HTMLElement(tagName) {}
};
/*-----------------------------------------------------------------*/
class HTMLCollection {
public:
	Node item(unsigned long int index);
	Node namedItem(std::string name);
private:
	unsigned long int length;
};
class HTMLDocument : public Document {
public:
	void open();
	void close();
	void write(std::string text);
	void writeln(std::string text);
	Element getElementById(std::string elementId);
	//NodeList getElementBYName(std::string elementName);

private:
	std::string title;
	std::string referrer;
	std::string domain;
	std::string URL;
	HTMLElement body;
	HTMLCollection images;
	HTMLCollection applets;
	HTMLCollection links;
	HTMLCollection forms;
	HTMLCollection anchors;
	std::string cookie;

};
class HTMLLinkElement : public HTMLElement {
	bool disabled;
	std::string charset;
	std::string href;
	std::string hreflang;
	std::string media;
	std::string rel;
	std::string rev;
	std::string target;
	std::string type;
};
class HTMLMetaElement : public HTMLElement {
	std::string content;
	std::string httpEquiv;
	std::string name;
	std::string scheme;
};
class HTMLBaseElement : public HTMLElement {
	std::string href;
	std::string target;
};
class HTMLIsIndexElement : public HTMLElement {
	std::string form;
	std::string prompt;
};
class HTMLStyleElement : public HTMLElement {
	bool disabled;
	std::string media;
	std::string type;
};
class HTMLFormElement : public HTMLElement {
public:
	void submit();
	void reset();
private:
	HTMLCollection elements;
	int length;
	std::string name;
	std::string acceptCharset;
	std::string action;
	std::string enctype;
	std::string method;
	std::string target;
};
class HTMLSelectElement : public HTMLElement {
public:
	void add(HTMLElement element, HTMLElement before);
	void remove(long int index);
	void blur();
	void focus();
private:
	std::string type;
	int selectedIndex;
	std::string value;
	int length;
	HTMLFormElement form;
	HTMLCollection options;
	bool disabled;
	bool multiple;
	std::string name;
	int size;
	int tabindex;
};
class HTMLOptGroupElement : public HTMLElement {
	bool disabled;
	std::string label;
};
class HTMLOptionElement : public HTMLElement {
	HTMLFormElement form;
	bool defaultSelected;
	std::string text;
	int index;
	bool disabled;
	std::string label;
	bool selected;
	std::string value;
};
class HTMLInputElement : public HTMLElement {
public:
	void blur();
	void focus();
	void select();
	void click();
private:
	std::string defaultValue;
	bool defaultChecked;
	HTMLFormElement form;
	std::string accept;
	std::string accesskey;
	std::string align;
	std::string alt;
	bool checked;
	bool disabled;
	int maxLength;
	std::string name;
	bool readOnly;
	std::string size;
	std::string src;
	int tabIndex;
	std::string type;
	std::string useMap;
	std::string value;
};
class HTMLTextAreaElement : public HTMLElement {
public:
	void blur();
	void focus();
	void select();
private:
	HTMLFormElement form;
	std::string defaultValue;
	std::string accessKey;
	int cols;
	bool disabled;
	std::string name;
	bool readOnly;
	int rows;
	int tabIndex;
	std::string type;
	std::string value;
};
class HTMLButtonElement : public HTMLElement {
	HTMLFormElement form;
	std::string accessKey;
	bool disabled;
	std::string name;
	int tabIndex;
	std::string type;
	std::string value;
};
class HTMLFieldSetElement : public HTMLElement {
	HTMLFormElement form;
};
class HTMLLabelElement : public HTMLElement {
	HTMLFormElement form;
	std::string accessKey;
	std::string htmlFor;
};
class HTMLLegendElement : public HTMLElement {
	HTMLFormElement form;
	std::string accessKey;
	std::string align;
};
class HTMLUListElement : public HTMLElement {
	bool compact;
	std::string type;
};
class HTMLOListElement : public HTMLElement {
	bool compact;
	int start;
	std::string type;
};
class HTMLDListElement : public HTMLElement {
	bool compact;
};
class HTMLDirectoryElement : public HTMLElement {
	bool comapct;
};
class HTMLMenuElement : public HTMLElement {
	bool comapct;
};
class HTMLLIElement : public HTMLElement {
	std::string type;
	int value;
};
class HTMLBlockquoteElement : public HTMLElement {
	std::string cite;
};
class HTMLQuoteElement : public HTMLElement {
	std::string cite;
};
class HTMLPreElement : public HTMLElement {
	int width;
};
class HTMLBRElement : public HTMLElement {
	std::string clear;
};
class HTMLBaseFrontElement : public HTMLElement {
	std::string color;
	std::string face;
	std::string size;
};
class HTMLFontElement : public HTMLElement {
	std::string color;
	std::string face;
	std::string size;
};
class HTMLHRElement : public HTMLElement {
	std::string align;
	bool noShade;
	std::string size;
	std::string width;
};
class HTMLModElement : public HTMLElement {
	std::string cite;
	std::string dateTime;
};
class HTMLImageElement : public HTMLElement {
	std::string lowSrc;
	std::string name;
	std::string align;
	std::string alt;
	std::string border;
	std::string height;
	std::string hspace;
	bool isMap;
	std::string longDesc;
	std::string src;
	std::string useMap;
	std::string vspace;
	std::string width;
};
class HTMLObjectElement : public HTMLElement {
	HTMLFormElement form;
	std::string code;
	std::string align;
	std::string archive;
	std::string border;
	std::string codeBase;
	std::string codeType;
	std::string data;
	bool declare;
	std::string height;
	std::string hspace;
	std::string name;
	std::string standby;
	int tabIndex;
	std::string type;
	std::string useMap;
	std::string vspace;
	std::string width;
};
class HTMLParamElement : public HTMLElement {
	std::string name;
	std::string type;
	std::string value;
	std::string valueType;
};
class HTMLAppletElement : public HTMLElement {
	std::string align;
	std::string alt;
	std::string archive;
	std::string code;
	std::string codeBase;
	std::string height;
	std::string hspace;
	std::string name;
	std::string object;
	std::string vspace;
	std::string width;
};
class HTMLMapElement : public HTMLElement {
	HTMLCollection areas;
	std::string name;
};
class HTMLAreaElement : public HTMLElement {
	std::string accessKey;
	std::string alt;
	std::string coords;
	std::string href;
	bool noHref;
	std::string shape;
	int tabIndex;
	std::string target;
};
class HTMLScriptElement : public HTMLElement {
	std::string text;
	std::string htmlFor;
	std::string event;
	std::string charset;
	bool defer;
	std::string src;
	std::string type;

};
class HTMLTableCaptionElement : public HTMLElement {
	std::string align;
};
class HTMLTableColElement : public HTMLElement {
	std::string align;
	std::string ch;
	std::string chOff;
	int span;
	std::string vAlign;
	std::string width;
};
class HTMLTableSectionElement : public HTMLElement {
public:
	HTMLElement insertRow(int index);
	void deleteRow(int index);
private:
	std::string align;
	std::string ch;
	std::string chOff;
	std::string vAlign;
	HTMLCollection rows;
};
class HTMLTableRowElement : public HTMLElement {
public:
	HTMLElement insertCell(int index);
	void deleteCell(int index);
private:
	int rowIndex;
	int sectionRowIndex;
	HTMLCollection cells;
	std::string align;
	std::string bgcolor;
	std::string ch;
	std::string chOff;
	std::string vAlign;
};
class HTMLTableCellElement : public HTMLElement {
	int cellIndex;
	std::string abbr;
	std::string align;
	std::string axis;
	std::string bgColor;
	std::string ch;
	std::string chOff;
	int colSpan;
	std::string headers;
	std::string height;
	bool noWrap;
	int rowSpan;
	std::string scope;
	std::string vAlign;
	std::string width;
};
class HTMLTableElement : public HTMLElement {
public:
	HTMLElement createTHead();
	void deleteTHead();
	HTMLElement createTFoot();
	void deleteTFoot();
	HTMLElement createCaption();
	void deleteCaption();
	HTMLElement insertRow(int index);
	void deleteRow(int index);
private:
	HTMLTableCaptionElement caption;
	HTMLTableSectionElement tHead;
	HTMLTableSectionElement tfoot;
	HTMLCollection rows;
	HTMLCollection tBodies;
	std::string align;
	std::string bgColor;
	std::string border;
	std::string cellpadding;
	std::string cellSpacing;
	std::string frame;
	std::string rules;
	std::string summary;
	std::string width;
};
class HTMLFrameSetElement : public HTMLElement {
	std::string cols;
	std::string rows;
};
class HTMLFrameElement : public HTMLElement {
	std::string frameBorder;
	std::string longDesc;
	std::string marginHeight;
	std::string marginWidth;
	std::string name;
	bool noResize;
	std::string scrolling;
	std::string src;
};

class HTMLIFrameElement : public HTMLElement {
	std::string align;
	std::string frameBorder;
	std::string height;
	std::string longDesc;
	std::string marginHeight;
	std::string marginWidth;
	std::string name;
	std::string scrolling;
	std::string src;
	std::string width;
};

