#include<iostream>
#include<SDL_ttf.h>
#include<algorithm>
//#include"winuser.h"
#include "window.h"
#include "rendertree.h"
#define endl '\n'
using std::cout;

void Window::test() {
	int w, h;
	SDL_GetWindowSize(mwindow, &w, &h);
	cout << w << "  width  " << h << " height " << endl;
	//mfont.test("Hello");
	//mfont.test("iiiii");
	//mfont.test("abcde");
	//mfont.test("nepal");
	//mfont.test("zzzzz");
	//mfont.test("ooooo");
}

Window::Window(int scrollBarWidth) :
	mwindow{ nullptr },
	mrenderer{ nullptr },
	mtexture{ nullptr }
{
	init();
	SDL_GetWindowSize(mwindow, &mSCREEN_WIDTH, &mSCREEN_HEIGHT);
	mscrollbar = Scrollbar(scrollBarWidth, mSCREEN_WIDTH, mSCREEN_HEIGHT);
	//mfont.loadFont("Roboto");
}

Window::~Window() {
	cout << "closing window" << endl;
	for (auto font : mFonts) {
		font.freeFont();
	}
	close();
}
bool Window::init() {
	bool	success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL couldnot be initialized\n" << SDL_GetError() << endl;
		success = false;
	}
	//creating window
	mwindow = SDL_CreateWindow("HTMLER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mSCREEN_WIDTH, mSCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_SHOWN);
	if (mwindow == NULL) {
		cout << "window couldnot be created " << SDL_GetError() << endl;
		success = false;
	}
	else {
		mrenderer = SDL_CreateRenderer(mwindow, -1, SDL_RENDERER_PRESENTVSYNC);
		if (mrenderer == NULL) {
			cout << "Couldnot create renderer " << SDL_GetError() << endl;
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(mrenderer, 255, 255, 255, 255);
			if (TTF_Init() == -1) {
				cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	return success;
}
void Window::close() {
	//deallocate surface
	SDL_DestroyRenderer(mrenderer);
	mrenderer = nullptr;
	//destroy window
	SDL_DestroyWindow(mwindow);
	mwindow = nullptr;
	//destroy window
	//SDL_FreeSurface(gimage);
	//gimage = NULL;
	//quitting SDL subsystem
	//IMG_Quit();
}
void Window::eventloop(RenderTree* tree) {
	bool quit = false;
	SDL_Event event;
	int currentypos{ 0 }; //for scrolling
	const int contentWidth = tree->children[0]->rect.w;
	const int contentHeight = tree->children[0]->rect.h;

	SDL_Texture* result = SDL_CreateTexture(mrenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, contentWidth, contentHeight);//currentRenderingtexture
	SDL_SetRenderTarget(mrenderer, result);
	render(tree->children[0]);

	mscrollbar.setTotalHeight(contentHeight, mSCREEN_HEIGHT);

	SDL_SetRenderTarget(mrenderer, nullptr);//setting the target to window
	const SDL_Rect DestRect = {
		0,
		0,
		std::min(mSCREEN_WIDTH,contentWidth),
		std::min(mSCREEN_HEIGHT,contentHeight)
	};

	while (!quit) {//main loop

		while (SDL_PollEvent(&event) != 0) {//event loop 
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_q:
					quit = true;
					break;
				case SDLK_d:
				case SDLK_DOWN:
					if (contentHeight > mSCREEN_HEIGHT) {
						currentypos += 100;
						currentypos = std::min(currentypos, contentHeight - mSCREEN_HEIGHT);
					}
					break;
				case SDLK_j:
					if (contentHeight > mSCREEN_HEIGHT) {
						currentypos += 20;
						currentypos = std::min(currentypos, contentHeight - mSCREEN_HEIGHT);
					}
					break;
				case SDLK_u:
				case SDLK_UP:
					currentypos -= 100;
					currentypos = std::max(0, currentypos);
					break;
				case SDLK_k:
					currentypos -= 20;
					currentypos = std::max(0, currentypos);
					break;
				}
				break;
			case SDL_MOUSEWHEEL:
			{
				if (event.wheel.y < 0)
				{
					if (contentHeight > mSCREEN_HEIGHT) {
						currentypos += 100;
						currentypos = std::min(currentypos, contentHeight - mSCREEN_HEIGHT);
					}
				}
				else if (event.wheel.y > 0)
				{
					currentypos -= 100;
					currentypos = std::max(0, currentypos);
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int pos = mscrollbar.handleMouseClick(event);
					cout << pos << endl;
					if (pos == 1)
					{
						if (contentHeight > mSCREEN_HEIGHT) {
							currentypos += mSCREEN_HEIGHT / 2;
							currentypos = std::min(currentypos, contentHeight - mSCREEN_HEIGHT);
						}
					}
					else if (pos == -1)
					{
						currentypos -= mSCREEN_HEIGHT / 2;
						currentypos = std::max(0, currentypos);
					}
				}
			}
			break;
			case SDL_MOUSEMOTION:
			{
				cout << event.motion.x << endl;
				bool hasHoverEffect = this->handleHover(tree->children[0], event, currentypos);
				if (hasHoverEffect) {
					SDL_SetRenderTarget(mrenderer, result);
					render(tree->children[0]);
					SDL_SetRenderTarget(mrenderer, nullptr);

				}

				cout << std::boolalpha << "hasHoverEffect " << hasHoverEffect << endl;
			}
			break;
			}
		}

		const SDL_Rect sourceRect = {
			0,
			currentypos,
			std::min(mSCREEN_WIDTH,contentWidth),
			std::min(mSCREEN_HEIGHT,contentHeight)
		};

		SDL_RenderClear(mrenderer);

		SDL_RenderCopy(mrenderer, result, &sourceRect, &DestRect);
		mscrollbar.render(mrenderer, currentypos, mSCREEN_WIDTH, mSCREEN_HEIGHT);
		SDL_RenderPresent(mrenderer);
	}
}

/*
returns true if atleast one including has an effect on hover
*/
bool Window::handleHover(RenderTree* tree, const SDL_Event& event, int currYPos) {
	/*first check for parent if the mouse lies in parent iterate through child
	else go to next sibling
	*/
	bool hasEffect = tree->hoverStyle.hasEffect();
	for (auto childtree : tree->children) {
		if (isPointInsideRect(childtree->rect, event.motion.x, event.motion.y + currYPos)) {
			childtree->hoverStyle.activeState = true;
			hasEffect = hasEffect || handleHover(childtree, event, currYPos);
			break;
		}
	}
	return hasEffect;
}

bool Window::isPointInsideRect(const SDL_Rect& rect, int x, int y) {
	if (x > rect.x && x<rect.x+rect.w && y>rect.y && y < rect.y+rect.h)
		return true;
	else
		return false;
}

void Window::getWindowSize(int* w) {
	SDL_GetWindowSize(mwindow, w, nullptr);
}

std::pair<int, int> Window::getFontSize(const std::string& text, const std::string& fontname, int fontsize) {
	auto fptr = this->getFontPtr(fontname, fontsize);
	int w, h;
	try {

		if (TTF_SizeText(fptr, text.c_str(), &w, &h) == 0) {
			return { w,h };
		}
		else {
			cout << "Error calculating text width and height: " << TTF_GetError() << endl;
			exit(EXIT_FAILURE);
		};
	}
	catch (char* ex) {
		cout << "EXCEPTION: " << ex << endl;
	}

}
/*
To render an element first render it and then render its child
To render itself we require the rect it convers
the x, y, w and h value of the rect is already calculated
iterate through the child  nodes :
if child node is text node find its x,y width and height and render it
if child node is element node call render(rendertree)
*/

void Window::render( RenderTree* tree) {
	//free texture
	if (mtexture != nullptr) {
		SDL_DestroyTexture(mtexture);
		mtexture = nullptr;
	}
	//set draw color to the background color of the element

	SDL_Color bgColor(tree->hoverStyle.activeState &&
		tree->hoverStyle.mbackgroundColor.first ?
		tree->hoverStyle.mbackgroundColor.second :
		tree->getBgColor()
	);

	SDL_SetRenderDrawColor(mrenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(mrenderer, &tree->rect);
	this->renderBox(tree);

	auto isTextNode = [](Node* node) {
		if (dynamic_cast<Text*>(node))
			return true;
		else
			return false;
	};

	auto& childNodes = tree->element->childNodes;

	int i = 0;
	int sumOfSiblingsHeight{ 0 };
	for (auto node : childNodes) {
		//iterating throught the childNode of current tree
		//calculate the width and height of line of the required text
		//get Parent width(assume block level)
		//this->render box() and find the content width 
		//now calculate the content height 
		//then calculate remaining
		auto textNodeptr = dynamic_cast<Text*>(node);
		if (textNodeptr) {
			//curr Node is textNode
			auto& text = textNodeptr->getText();
			auto [width, height] = RenderTree::windowptr->getFontSize(text, tree->getFontName(), tree->getFontSize());
			int textwidth = tree->rect.w -
				tree->styles->mpadding.left.toPixel() -
				tree->styles->mpadding.right.toPixel() -
				tree->styles->mborder.left.toPixel() -
				tree->styles->mborder.right.toPixel();
			int linecount = static_cast<int>(width / textwidth);
			if (width > textwidth * linecount) {
				linecount++;
			}
 
			SDL_Color textColor{ tree->hoverStyle.activeState &&
				tree->hoverStyle.mcolor.first ?
				tree->hoverStyle.mcolor.second :
				tree->getColor()
			};
			auto fptr = this->getFontPtr(tree->getFontName(), tree->getFontSize());
			//SDL_Surface* textSurface = TTF_RenderText_Shaded_Wrapped(fptr, text.c_str(), SDL_Color{}, textColor, textwidth);
			SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(fptr, text.c_str(), textColor, textwidth);
			if (textSurface == nullptr) {
				cout << "cannot load text" << endl;
				exit(EXIT_FAILURE);
			}

			SDL_Rect textRect = {
				tree->rect.x + tree->styles->mpadding.left.toPixel() + tree->styles->mborder.left.toPixel(),
				tree->rect.y + tree->styles->mpadding.top.toPixel() + tree->styles->mborder.top.toPixel(),
				width < textwidth ? width : textwidth,
				linecount* height
			};

			/*
			The x and y for the text node will be its containing blocks x,y - its padding and border in corr direction
			*/
			//sumOfSiblingsHeight += linecount * height;
			mtexture = SDL_CreateTextureFromSurface(mrenderer, textSurface);
			SDL_RenderCopy(mrenderer, mtexture, NULL, &textRect);
			SDL_FreeSurface(textSurface);

		}
		else {
			//if not text node apply recursion
			render(tree->children[i++]);
			//sumOfSiblingsHeight += tree->children[i - 1]->rect.h;
		}
	}
	tree->hoverStyle.activeState = false;
}

void Window::renderBox(const RenderTree* tree) {
	constexpr int fontSize = 18;
	struct FourRect {
		SDL_Rect top, right, bottom, left;
	};
	struct FourRect margin, padding, border;
	SDL_Rect content;
	//assume border-box for now
	//no margin for now
	border.left.x = tree->rect.x;
	border.left.y = tree->rect.y;
	border.left.w = tree->styles->mborder.left.toPixel();
	border.left.h = tree->rect.h;

	border.right.x = tree->rect.x + tree->rect.w - tree->styles->mborder.right.toPixel();
	border.right.y = tree->rect.y;
	border.right.w = tree->styles->mborder.right.toPixel();
	border.right.h = tree->rect.h;

	border.top.x = tree->rect.x;
	border.top.y = tree->rect.y;
	border.top.w = tree->rect.w;
	border.top.h = tree->styles->mborder.top.toPixel();

	border.bottom.x = tree->rect.x;
	border.bottom.y = tree->rect.y + tree->rect.h - tree->styles->mborder.bottom.toPixel();
	border.bottom.w = tree->rect.w;
	border.bottom.h = tree->styles->mborder.bottom.toPixel();

	padding.left.x = tree->rect.x + border.left.w;
	padding.left.y = border.top.y + border.top.h;
	padding.left.w = tree->styles->mpadding.left.toPixel();
	padding.left.h = tree->rect.h - border.top.h - border.bottom.h;

	padding.right.x = border.right.x - tree->styles->mpadding.right.toPixel();
	padding.right.y = border.top.y + border.top.h;
	padding.right.w = tree->styles->mpadding.right.toPixel();
	padding.right.h = tree->rect.h - border.top.h - border.bottom.h;



	padding.top.x = padding.left.x;
	padding.top.y = padding.right.y;
	padding.top.w = border.top.w - border.left.w - border.right.w;
	padding.top.h = tree->styles->mpadding.top.toPixel();

	padding.bottom.x = padding.left.x;
	padding.bottom.y = border.bottom.y - tree->styles->mpadding.bottom.toPixel();
	padding.bottom.w = padding.top.w;
	padding.bottom.h = tree->styles->mpadding.bottom.toPixel();

	content.x = padding.left.x + padding.left.w;
	content.y = padding.top.y + padding.top.h;
	content.w = padding.top.w - padding.left.w - padding.right.w;
	content.h = padding.left.h - padding.top.h - padding.bottom.h;



	//if border-color apply border color
	//else border-color=text color
	if (border.right.w != 0) {
		SDL_SetRenderDrawColor(mrenderer, tree->styles->mborder.right.borderColor.r, tree->styles->mborder.right.borderColor.g, tree->styles->mborder.right.borderColor.b, 255);
		SDL_RenderFillRect(mrenderer, &border.right);
	}
	if (border.top.h != 0) {
		SDL_SetRenderDrawColor(mrenderer, tree->styles->mborder.top.borderColor.r, tree->styles->mborder.top.borderColor.g, tree->styles->mborder.top.borderColor.b, 255);
		SDL_RenderFillRect(mrenderer, &border.top);
	}
	if (border.left.w != 0) {
		SDL_SetRenderDrawColor(mrenderer, tree->styles->mborder.left.borderColor.r, tree->styles->mborder.left.borderColor.g, tree->styles->mborder.left.borderColor.b, 255);
		SDL_RenderFillRect(mrenderer, &border.left);
	}
	if (border.bottom.h != 0) {
		SDL_SetRenderDrawColor(mrenderer, tree->styles->mborder.bottom.borderColor.r, tree->styles->mborder.bottom.borderColor.g, tree->styles->mborder.bottom.borderColor.b, 255);
		SDL_RenderFillRect(mrenderer, &border.bottom);
	}
}

void Window::setRootColor() {
	SDL_SetRenderDrawColor(mrenderer, 255, 255, 255, 255);
}

TTF_Font* Window::getFontPtr(std::string fontname, int fontsize) {
	auto font = std::find_if(mFonts.begin(), mFonts.end(), [fontname](const Font& font) {
		return (font.mname == fontname);
		});
	if (font != mFonts.end())
	{
		return font->getFontPtr(fontsize);
	}
	else {
		mFonts.push_back(Font{ fontname });
		return mFonts.back().getFontPtr(fontsize);
	}
}


/*===================================================================*/
/**************************** FONT CLASS *****************************/
/*===================================================================*/


TTF_Font* Font::getFontPtr(int fontsize) {
	auto font = std::find_if(fontsizeptrs.begin(), fontsizeptrs.end(), [fontsize](std::pair<int, TTF_Font*> font) {
		return (font.first == fontsize);
		});
	if (font != fontsizeptrs.end())
	{
		return font->second;
	}
	else {
		auto fptr = this->loadFont(fontsize);
		fontsizeptrs.push_back(std::pair<int, TTF_Font*>{fontsize, fptr});
		return fptr;
	}
}

Font::Font(const std::string& name) :mname{ name } {
}
Font::~Font() {
	//leave it here for now//warning
}
void Font::freeFont() {
	cout << "freeing font  " << mname << endl;
	for (auto fptr : fontsizeptrs) {
		cout << "\tsize " << fptr.first << endl;
		TTF_CloseFont(fptr.second);

	}

}

TTF_Font* Font::loadFont(const std::string& n, int fontsize) {
	TTF_Font* fptr = TTF_OpenFont(("../fonts/" + n + ".ttf").c_str(), fontsize);
	if (fptr == nullptr) {
		cout << "couldnot load font: " << TTF_GetError() << endl;
	}
	return fptr;
}
TTF_Font* Font::loadFont(int fontsize) {
	return loadFont(mname, fontsize);
}
void Font::test(const std::string& t) {

}




//TODO: active formatting elements
