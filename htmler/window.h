#pragma once
#include<vector>
#include<SDL.h>
#include<SDL_ttf.h>
#include "scrollbar.h"

class RenderTree;
class Window;
class Font {
private:
	std::string mname;
	//TTF_Font* mfont;
	//int size{ 18 };
	std::vector<std::pair<int, TTF_Font*>> fontsizeptrs;
public:
	friend class Window;
	Font(const std::string& name = "Roboto");
	~Font();
	void test(const std::string& t = "a");
	TTF_Font* loadFont(const std::string& n, int fontsize = 18);
	TTF_Font* loadFont(int fontsize);
	TTF_Font* getFontPtr(int fontsize);
	void freeFont();

};

class Window {
	int mSCREEN_WIDTH;
	int mSCREEN_HEIGHT;
	Scrollbar mscrollbar;
	SDL_Window* mwindow;
	SDL_Renderer* mrenderer;
	SDL_Texture* mtexture;
	//static std::string title;
	//std::vector<Font> fonts;
	//Font mfont;
	std::vector<Font> mFonts;
public:
	Window(int scrollbarwidth);
	~Window();
	bool init();
	void close();
	void eventloop(RenderTree*);
	void test();
	void getWindowSize(int* w);
	std::pair<int, int> getFontSize(const std::string& text, const std::string& fontname, int fontsize);
	void render(RenderTree* tree);
	TTF_Font* getFontPtr(std::string fontname, int fontsize);
	void renderBox(const RenderTree* tree);
	void 	setRootColor();
	bool handleHover(RenderTree* tree, const SDL_Event& event, int currYPos);
	bool isPointInsideRect(const SDL_Rect& rect, int x, int y);

};












