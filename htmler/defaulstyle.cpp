#include"defaultstyle.h"
#include"states.h"
inline 	int styles::Length::toPixel(int size)const {
	switch (this->mlengthType)
	{
	case styles::LengthType::PIXEL:
		return mvalue;
		break;
	case styles::LengthType::EM:
		return mvalue * size;//TODO: change
		break;
	case styles::LengthType::REM:
		return mvalue * size;
		break;
	case styles::LengthType::AUTO:
		//TODO: later if css implemented
		break;
		//case styles::LengthType::INHERIT:
			//TODO
			//break;
	case styles::LengthType::NOT_SPECIFIED:
		return 0;
		break;
	}
}

inline  int operator+(const Border& b1, const Border& b2) {
	return b1.borderWidth + b2.borderWidth;
}
int styles::Border::toPixel(int size) {
	return borderWidth.toPixel(size);
}
int styles::operator+(const styles::Length& l1, const styles::Length& l2)
{
	return 0;
}
Border styles::parseBorder(std::string bordertext) {
	trim(bordertext);
	/*
	0 - width
	1 - style
	2 - color
	*/
	std::vector<std::string >borderlist;
	borderlist.push_back("");
	for (char c : bordertext) {
		if (c == ' ') {
			borderlist.push_back("");
		}
		borderlist.back() += c;
	}
	Length bwidth{ getLength(borderlist[0]) ,getLengthType(borderlist[0]) };
	styles::BorderStyle bstyle = styles::BorderStyle::SOLID;
	if (borderlist[1] == "solid") {
	}
	else if (borderlist[1] == "dotted") {
		bstyle == styles::BorderStyle::DOTTED;
	}
	SDL_Color bcolor = styles::parseColor(borderlist[2]);

	return Border{ bwidth,bstyle,bcolor };
}
styles::Border::Border() = default;
styles::Border::Border(const Length & l, const BorderStyle s, const SDL_Color & c)
	:borderWidth{ l },
	borderStyle{ s },
	borderColor{ c }
{ }

styles::Length::Length(float val, LengthType lt) :
	mvalue{ val },
	mlengthType{ lt }
{
}
styles::Length::Length() :
	mvalue{ 0 }, mlengthType{ LengthType::NOT_SPECIFIED }{}
int operator+(const styles::Length & l1, const styles::Length & l2) {
	return l1.toPixel() + l2.toPixel();
}

static float styles::getLength(const std::string & str) {
	try
	{
		const float i{ std::stof(str) };
		return i;
	}
	catch (std::invalid_argument const& ex)
	{
		cout << "Invalid Length" << endl;
		cout << ex.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch (std::out_of_range const& ex)
	{
		cout << "Invalid Length" << endl;
		cout << ex.what() << endl;
		exit(EXIT_FAILURE);
	}
}

styles::LengthType styles::getLengthType(const std::string & str) {
	if (str.ends_with("px"))
		return LengthType::PIXEL;
	else if (str.ends_with("rem"))//should be above em 
		return LengthType::REM;
	else if (str.ends_with("em"))
		return LengthType::EM;
}
styles::Length styles::parseLength(const std::string & length) {
	//MUST : validate length
	float value{ getLength(length) };
	LengthType ltype = value == 0 ? LengthType::PIXEL : getLengthType(length);
	return Length{ value,ltype };
}
struct styles::Dimension styles::parseLengthList(std::string lengths) {
	trim(lengths);
	std::vector<std::string >lengthList;
	lengthList.push_back("");
	for (char c : lengths) {
		if (c == ' ') {
			lengthList.push_back("");
		}
		lengthList.back() += c;
	}

	if (lengthList.size() == 1) {
		Length length = parseLength(lengthList[0]);
		return Dimension{
		length,
		length,
		length,
		length
		};
	};

	if (lengthList.size() == 2) {
		Length ylength = parseLength(lengthList[0]);
		Length xlength = parseLength(lengthList[1]);
		return Dimension{
			.top = ylength,
			.right = xlength,
			.bottom = ylength,
			.left = xlength
		};
	};

	if (lengthList.size() == 3) {
		Length ytlength = parseLength(lengthList[0]);
		Length xlength = parseLength(lengthList[1]);
		Length yblength = parseLength(lengthList[2]);
		return Dimension{
			.top = ytlength,
			.right = xlength,
			.bottom = yblength,
			.left = xlength
		};
	};

	if (!(lengthList.size() == 4)) {
		cout << "Invlaud Length Value";
		exit(EXIT_FAILURE);
	}
	//if size==4;
	Dimension dimension;
	for (int i{}; i < 4; i++) {
		Length length = parseLength(lengthList[i]);
		switch (i) {
		case 0:dimension.top = length; break;
		case 1:dimension.right = length; break;
		case 2:dimension.bottom = length; break;
		case 3:dimension.left = length; break;
		}
	}
}

SDL_Color styles::parseColor(std::string & clr) {
	trim(clr);
	if (clr.starts_with('#')) {
		if (clr.empty()) {
			cout << "INVALID COLOR" << endl;
			exit(EXIT_FAILURE);
		}
		clr.erase(0, 1);
		try {
			if (clr.size() == 3) {
				uint8_t r = std::stoi(std::string(2, clr[0]), nullptr, 16);
				uint8_t g = std::stoi(std::string(2, clr[1]), nullptr, 16);
				uint8_t b = std::stoi(std::string(2, clr[2]), nullptr, 16);
				return { r,g,b };
			}
			if (clr.size() == 6) {
				uint8_t r = std::stoi(clr.substr(0, 2), nullptr, 16);
				uint8_t g = std::stoi(clr.substr(2, 2), nullptr, 16);
				uint8_t b = std::stoi(clr.substr(4, 2), nullptr, 16);

				return { r,g,b };
			}
		}
		catch (std::invalid_argument const& ex) {
			std::cout << "error: " << ex.what() << '\n';
		}
		cout << "INVALID Hex value of color" << endl;
		cout << " only 3 digit and 6 digit color value is supported now" << endl;
		exit(EXIT_FAILURE);
	}
	if (clr == "black")return { 0,0,0,255 };
	if (clr == "silver")return { 192,192,192,255 };
	if (clr == "gray")return { 128,128,128,255 };
	if (clr == "white")return { 255,255,255,255 };
	if (clr == "maroon")return { 128,0,0,255 };
	if (clr == "red")return { 255,0,0,255 };
	if (clr == "purple")return { 128,0,128,255 };
	if (clr == "fuchsia")return { 255,0,255,255 };
	if (clr == "green")return { 0,128,0,255 };
	if (clr == "lime")return { 0,255,0,255 };
	if (clr == "olive")return { 128,128,0,255 };
	if (clr == "yellow")return { 255,255,0,255 };
	if (clr == "navy")return { 0,0,128,255 };
	if (clr == "blue")return { 0,0,255,255 };
	if (clr == "teal")return { 0,128,128,255 };
	if (clr == "aqua")return { 0,255,255,255 };
	if (clr == "aliceblue")return { 240,248,255,255 };
	if (clr == "antiquewhite")return { 250,235,215,255 };
	if (clr == "aqua")return { 0,255,255,255 };
	if (clr == "aquamarine")return { 127,255,212,255 };
	if (clr == "azure")return { 240,255,255,255 };
	if (clr == "beige")return { 245,245,220,255 };
	if (clr == "bisque")return { 255,228,196,255 };
	if (clr == "black")return { 0,0,0,255 };
	if (clr == "blanchedalmond")return { 255,235,205,255 };
	if (clr == "blue")return { 0,0,255,255 };
	if (clr == "blueviolet")return { 138,43,226,255 };
	if (clr == "brown")return { 165,42,42,255 };
	if (clr == "burlywood")return { 222,184,135,255 };
	if (clr == "cadetblue")return { 95,158,160,255 };
	if (clr == "chartreuse")return { 127,255,0,255 };
	if (clr == "chocolate")return { 210,105,30,255 };
	if (clr == "coral")return { 255,127,80,255 };
	if (clr == "cornflowerblue")return { 100,149,237,255 };
	if (clr == "cornsilk")return { 255,248,220,255 };
	if (clr == "crimson")return { 220,20,60,255 };
	if (clr == "cyan")return { 0,255,255,255 };
	if (clr == "darkblue")return { 0,0,139,255 };
	if (clr == "darkcyan")return { 0,139,139,255 };
	if (clr == "darkgoldenrod")return { 184,134,11,255 };
	if (clr == "darkgray")return { 169,169,169,255 };
	if (clr == "darkgreen")return { 0,100,0,255 };
	if (clr == "darkgrey")return { 169,169,169,255 };
	if (clr == "darkkhaki")return { 189,183,107,255 };
	if (clr == "darkmagenta")return { 139,0,139,255 };
	if (clr == "darkolivegreen")return { 85,107,47,255 };
	if (clr == "darkorange")return { 255,140,0,255 };
	if (clr == "darkorchid")return { 153,50,204,255 };
	if (clr == "darkred")return { 139,0,0,255 };
	if (clr == "darksalmon")return { 233,150,122,255 };
	if (clr == "darkseagreen")return { 143,188,143,255 };
	if (clr == "darkslateblue")return { 72,61,139,255 };
	if (clr == "darkslategray")return { 47,79,79,255 };
	if (clr == "darkslategrey")return { 47,79,79,255 };
	if (clr == "darkturquoise")return { 0,206,209,255 };
	if (clr == "darkviolet")return { 148,0,211,255 };
	if (clr == "deeppink")return { 255,20,147,255 };
	if (clr == "deepskyblue")return { 0,191,255,255 };
	if (clr == "dimgray")return { 105,105,105,255 };
	if (clr == "dimgrey")return { 105,105,105,255 };
	if (clr == "dodgerblue")return { 30,144,255,255 };
	if (clr == "firebrick")return { 178,34,34,255 };
	if (clr == "floralwhite")return { 255,250,240,255 };
	if (clr == "forestgreen")return { 34,139,34,255 };
	if (clr == "fuchsia")return { 255,0,255,255 };
	if (clr == "gainsboro")return { 220,220,220,255 };
	if (clr == "ghostwhite")return { 248,248,255,255 };
	if (clr == "gold")return { 255,215,0,255 };
	if (clr == "goldenrod")return { 218,165,32,255 };
	if (clr == "gray")return { 128,128,128,255 };
	if (clr == "green")return { 0,128,0,255 };
	if (clr == "greenyellow")return { 173,255,47,255 };
	if (clr == "grey")return { 128,128,128,255 };
	if (clr == "honeydew")return { 240,255,240,255 };
	if (clr == "hotpink")return { 255,105,180,255 };
	if (clr == "indianred")return { 205,92,92,255 };
	if (clr == "indigo")return { 75,0,130,255 };
	if (clr == "ivory")return { 255,255,240,255 };
	if (clr == "khaki")return { 240,230,140,255 };
	if (clr == "lavender")return { 230,230,250,255 };
	if (clr == "lavenderblush")return { 255,240,245,255 };
	if (clr == "lawngreen")return { 124,252,0,255 };
	if (clr == "lemonchiffon")return { 255,250,205,255 };
	if (clr == "lightblue")return { 173,216,230,255 };
	if (clr == "lightcoral")return { 240,128,128,255 };
	if (clr == "lightcyan")return { 224,255,255,255 };
	if (clr == "lightgoldenrodyellow")return { 250,250,210,255 };
	if (clr == "lightgray")return { 211,211,211,255 };
	if (clr == "lightgreen")return { 144,238,144,255 };
	if (clr == "lightgrey")return { 211,211,211,255 };
	if (clr == "lightpink")return { 255,182,193,255 };
	if (clr == "lightsalmon")return { 255,160,122,255 };
	if (clr == "lightseagreen")return { 32,178,170,255 };
	if (clr == "lightskyblue")return { 135,206,250,255 };
	if (clr == "lightslategray")return { 119,136,153,255 };
	if (clr == "lightslategrey")return { 119,136,153,255 };
	if (clr == "lightsteelblue")return { 176,196,222,255 };
	if (clr == "lightyellow")return { 255,255,224,255 };
	if (clr == "lime")return { 0,255,0,255 };
	if (clr == "limegreen")return { 50,205,50,255 };
	if (clr == "linen")return { 250,240,230,255 };
	if (clr == "magenta")return { 255,0,255,255 };
	else if (clr == "maroon")return { 128,0,0,255 };
	if (clr == "mediumaquamarine")return { 102,205,170,255 };
	if (clr == "mediumblue")return { 0,0,205,255 };
	if (clr == "mediumorchid")return { 186,85,211,255 };
	if (clr == "mediumpurple")return { 147,112,219,255 };
	if (clr == "mediumseagreen")return { 60,179,113,255 };
	if (clr == "mediumslateblue")return { 123,104,238,255 };
	if (clr == "mediumspringgreen")return { 0,250,154,255 };
	if (clr == "mediumturquoise")return { 72,209,204,255 };
	if (clr == "mediumvioletred")return { 199,21,133,255 };
	if (clr == "midnightblue")return { 25,25,112,255 };
	if (clr == "mintcream")return { 245,255,250,255 };
	if (clr == "mistyrose")return { 255,228,225,255 };
	if (clr == "moccasin")return { 255,228,181,255 };
	if (clr == "navajowhite")return { 255,222,173,255 };
	if (clr == "navy")return { 0,0,128,255 };
	if (clr == "oldlace")return { 253,245,230,255 };
	if (clr == "olive")return { 128,128,0,255 };
	if (clr == "olivedrab")return { 107,142,35,255 };
	if (clr == "orange")return { 255,165,0,255 };
	if (clr == "orangered")return { 255,69,0,255 };
	if (clr == "orchid")return { 218,112,214,255 };
	if (clr == "palegoldenrod")return { 238,232,170,255 };
	if (clr == "palegreen")return { 152,251,152,255 };
	if (clr == "paleturquoise")return { 175,238,238,255 };
	if (clr == "palevioletred")return { 219,112,147,255 };
	if (clr == "papayawhip")return { 255,239,213,255 };
	if (clr == "peachpuff")return { 255,218,185,255 };
	if (clr == "peru")return { 205,133,63,255 };
	if (clr == "pink")return { 255,192,203,255 };
	if (clr == "plum")return { 221,160,221,255 };
	if (clr == "powderblue")return { 176,224,230,255 };
	if (clr == "purple")return { 128,0,128,255 };
	if (clr == "red")return { 255,0,0,255 };
	if (clr == "rosybrown")return { 188,143,143,255 };
	if (clr == "royalblue")return { 65,105,225,255 };
	if (clr == "saddlebrown")return { 139,69,19,255 };
	if (clr == "salmon")return { 250,128,114,255 };
	if (clr == "sandybrown")return { 244,164,96,255 };
	if (clr == "seagreen")return { 46,139,87,255 };
	if (clr == "seashell")return { 255,245,238,255 };
	if (clr == "sienna")return { 160,82,45,255 };
	if (clr == "silver")return { 192,192,192,255 };
	if (clr == "skyblue")return { 135,206,235,255 };
	if (clr == "slateblue")return { 106,90,205,255 };
	if (clr == "slategray")return { 112,128,144,255 };
	if (clr == "slategrey")return { 112,128,144,255 };
	if (clr == "snow")return { 255,250,250,255 };
	if (clr == "springgreen")return { 0,255,127,255 };
	if (clr == "steelblue")return { 70,130,180,255 };
	if (clr == "tan")return { 210,180,140,255 };
	if (clr == "teal")return { 0,128,128,255 };
	if (clr == "thistle")return { 216,191,216,255 };
	if (clr == "tomato")return { 255,99,71,255 };
	if (clr == "turquoise")return { 64,224,208,255 };
	if (clr == "violet")return { 238,130,238,255 };
	if (clr == "wheat")return { 245,222,179,255 };
	if (clr == "white")return { 255,255,255,255 };
	if (clr == "whitesmoke")return { 245,245,245,255 };
	if (clr == "yellow")return { 255,255,0,255 };
	if (clr == "yellowgreen")return { 154,205,50,255 };
	else {
		cout << "unknown color value" << endl;
		exit(EXIT_FAILURE);
	}
}

styles::FontStyle styles::parseFontStyle(const std::string & str) {
	if (str == "italic")return styles::FontStyle::ITALIC;
	styles::FontStyle::NORMAL;
}


