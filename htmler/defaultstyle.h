#pragma once
#include<vector>
#include<array>
#include<string>
#include<SDL.h>
#include<iostream>
#include"states.h"
#define endl '\n'
using std::cout;

namespace styles {
	enum class LengthType {
		PIXEL,
		EM,
		REM,
		AUTO,
		NOT_SPECIFIED
	};
	class Length {
	public:
		LengthType mlengthType;
		float mvalue;
	public:
		Length();
		Length(float val, LengthType lt = LengthType::PIXEL);
		inline 	int toPixel(int size = 18)const;
		friend int operator+(const styles::Length& l1, const styles::Length& l2);
	};
	enum  class Display {
		BLOCK,
		INLINE,
		INLINE_BLOCK,
		NONE,
		NOT_SPECIFIED,
		LIST_ITEM
	};
	enum class BoxSizing {
		CONTENTBOX,
		BORDERBOX,
	};
	enum class FontStyle {
		NOT_SPECIFIED,
		NORMAL,
		ITALIC,
	};
	enum class BorderStyle {
		SOLID,
		DOTTED,
		INSET
	};
	enum class TextDecoration { // Conflict
		OVERLINE,
		LINETHROUGH,
		UNDERLINE,
		NOT_SPECIFIED
	};
	enum class FontWeight {
		NORMAL,
		BOLD,
		LIGHT,
		LIGHTER,
		NOT_SPECIFIED
	};
	enum class ListStyleType {
		DECIMAL,
		DISC,
		NOT_SPECIFIED
	};
	class Border {
	public:
		Length borderWidth;
		BorderStyle borderStyle;
		SDL_Color borderColor;
	public:
		Border();
		Border(const Length& l, const BorderStyle s, const SDL_Color& c);
		friend inline  int operator+(const Border& b1, const Border& b2);
		int toPixel(int size = 18);
	};
	Border parseBorder(std::string bordertext);

	FontStyle parseFontStyle(const std::string& str);
	struct Dimension {
		Length top;
		Length right;
		Length bottom;
		Length left;
	};
	static float getLength(const std::string& str);
	LengthType getLengthType(const std::string& str);
	Length parseLength(const std::string& length);
	struct Dimension parseLengthList(std::string lengths);
	struct BorderData {
		Border top;
		Border right;
		Border bottom;
		Border left;
	};
	SDL_Color parseColor(std::string& clr);

};

using namespace styles;
class DynamicStyle {
public:
	bool activeState{ false };
	std::pair<bool, SDL_Color>  mcolor{ false,{} };
	std::pair<bool, SDL_Color>  mbackgroundColor{ false,{} };
	bool hasEffect() const{
		return (mcolor.first || mbackgroundColor.first);
	}

};
class Style {
public:
	styles::TextDecoration mTextDecoration{ TextDecoration::NOT_SPECIFIED };
	styles::FontWeight mFontWeight{ FontWeight::NOT_SPECIFIED };
	styles::Display mdisplay{ Display::NOT_SPECIFIED };
	styles::ListStyleType mliststyletype{ ListStyleType::NOT_SPECIFIED };
	styles::Length mfontSize;
	std::string mfontName;
	SDL_Color mcolor;
	SDL_Color mbackgroundColor;
	styles::FontStyle mfontStyle{ FontStyle::NOT_SPECIFIED };
	styles::Dimension mmargin;
	styles::Dimension mpadding;
	styles::BorderData mborder;
	//styles::Dimension mborder;
	//std::array<styles::Border, 4> mborder;
	styles::Display getDisplay() {
		return mdisplay;
	}
};

namespace SS {
	using namespace styles;
	class HTMLDivStyle :public Style {
	public:
		HTMLDivStyle() {
			mdisplay = Display::BLOCK;
		}
	};
	class  HTMLHtmlStyle :public Style {
	public:
		HTMLHtmlStyle() {

		}
	};
	class HTMLPStyle :public Style {
	public:
		HTMLPStyle() {
			mdisplay = Display::BLOCK;
			mmargin = { {1,LengthType::EM},{},{1,LengthType::EM} ,{} };
		}
	};
	class HTMLLiStyle :public Style {
	public:
		HTMLLiStyle() {
			mdisplay = Display::LIST_ITEM;
		}
	};
	class HTMLOlStyle :public Style {
	public:
		HTMLOlStyle() {
			mdisplay = Display::BLOCK;
			mliststyletype = ListStyleType::DECIMAL;
			mmargin = { {1,LengthType::EM},{},{1,LengthType::EM} ,{} };
			mpadding = { Length(),Length(),Length(),Length(40,LengthType::PIXEL) };//pixel
		}
	};
	class HTMLUlStyle :public Style {
	public:
		HTMLUlStyle() {
			mdisplay = Display::BLOCK;
			mliststyletype = ListStyleType::DISC;
			mmargin = { Length(1,LengthType::EM),Length(0,LengthType::EM),Length(1,LengthType::EM),Length(0,LengthType::EM) };
			mpadding = { Length(),Length(),Length(),Length(40,LengthType::PIXEL) };
		}
	};
	class HTMLArticleStyle :public Style {
	public:
		HTMLArticleStyle() {
			mdisplay = Display::BLOCK;
		}
	};
	class HTMLUStyle :public Style {
	public:
		HTMLUStyle() {
			mTextDecoration = TextDecoration::UNDERLINE;
		}
	};
	class HTMLStyleStyle :public Style {
	public:
		HTMLStyleStyle() {
			mdisplay = Display::NONE;
		}
	};
	class HTMLStrongStyle :public Style {
	public:
		HTMLStrongStyle() {
			mFontWeight = FontWeight::BOLD;
		}
	};
	class HTMLStrikeStyle :public Style {
	public:
		HTMLStrikeStyle() {
			mTextDecoration = TextDecoration::LINETHROUGH;
		}
	};
	class HTMLNavStyle :public Style {
	public:
		HTMLNavStyle() {
			mdisplay = Display::BLOCK;
		}
	};
	class HTMLHrStyle :public Style {
	public:
		HTMLHrStyle() {
			mdisplay = Display::BLOCK;
			mmargin = { Length(0.5,LengthType::EM),Length(0,LengthType::AUTO),Length(0.5,LengthType::EM),Length(0,LengthType::AUTO) }; // auto {0.5 em ,auto ,0.5em ,auto }
			//border style = inset;
			// Border to add
			//mborder={Length(1,LengthType::PIXEL),INSET}
			//TO
		}
	};
	class HTMLEmStyle :public Style {
	public:
		HTMLEmStyle() {
			mfontStyle = FontStyle::ITALIC;
		}
	};
	class HTMLH1Style :public Style {
	public:
		HTMLH1Style() {
			mdisplay = Display::BLOCK;
			mfontSize = Length(2, LengthType::EM);
			mmargin = { Length(0.67,LengthType::EM),Length(),Length(0.67,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;

		}
	};
	class HTMLH2Style :public Style {
	public:
		HTMLH2Style() {
			mdisplay = Display::BLOCK;
			mfontSize = Length(1.5, LengthType::EM);
			mmargin = { Length(0.83,LengthType::EM),Length(),Length(0.83,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;

		}
	};
	class HTMLH3Style :public Style {
	public:
		HTMLH3Style() {
			mdisplay = Display::BLOCK;
			mfontSize = Length(1.17, LengthType::EM);
			mmargin = { Length(1,LengthType::EM),Length(),Length(1,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;

		}
	};
	class HTMLH4Style :public Style {
	public:
		HTMLH4Style() {
			mdisplay = Display::BLOCK;
			mmargin = { Length(1.33,LengthType::EM),Length(),Length(1.33,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;

		}
	};
	class HTMLH5Style :public Style {
	public:
		HTMLH5Style() {
			mdisplay = Display::BLOCK;
			mfontSize = Length(0.83, LengthType::EM);
			mmargin = { Length(1.67,LengthType::EM),Length(),Length(1.67,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;

		}
	};
	class HTMLH6Style :public Style {
	public:
		HTMLH6Style() {
			mdisplay = Display::BLOCK;
			mfontSize = Length(0.67, LengthType::EM);
			mmargin = { Length(2.33,LengthType::EM),Length(),Length(2.33,LengthType::EM),Length() };
			mFontWeight = FontWeight::BOLD;
		}
	};

	//TODO: assign member
	class HTMLTitleStyle :public Style {
	public:
		HTMLTitleStyle() {
			mdisplay = Display::NONE;
		}
	};
	class HTMLMetaStyle :public Style {
	public:
		HTMLMetaStyle() {
			mdisplay = Display::NONE;
		}
	};
	class HTMLHeadStyle :public Style {
	public:
		HTMLHeadStyle() {
			mdisplay = Display::NONE;
		}
	};
	class HTMLBodyStyle :public Style {
	public:
		HTMLBodyStyle() {
			//mdisplay = Display::NONE;
		}
	};
};
//Elements with no default style 
//SPAN, META, MAIN, BUTTON, BR
