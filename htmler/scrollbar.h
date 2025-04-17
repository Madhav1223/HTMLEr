#pragma once
#include<iostream>
#include<SDL.h>

class Scrollbar {
private:
	int mwidth;
	int mheight;
	int mcurrYPos;
	int mcontentHeight;
	SDL_Rect mscrollBackgroundRect;
public:
	Scrollbar() = default;
	Scrollbar(int w,int screenWidth,int screenHeight) :
		mwidth(w),
		mcurrYPos(0), 
		mscrollBackgroundRect{
			screenWidth - mwidth,
			0,
			mwidth,
			screenHeight
	} {}
	void setTotalHeight(int height,int screenHeight) {
		mcontentHeight = height;
		mheight = screenHeight / (static_cast<float>(mcontentHeight) / screenHeight);
	}
	void render(SDL_Renderer* renderer,int currYPos,int screenWidth,int screenHeight) {
		
		int paddingX = 2;
		mcurrYPos = (static_cast<float>(currYPos) / mcontentHeight) * screenHeight;
		SDL_Rect scrollBarRect{
			screenWidth-mwidth+paddingX,
			mcurrYPos,
			mwidth-2*paddingX,
			mheight
		};
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderFillRect(renderer, &mscrollBackgroundRect);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &scrollBarRect);


	}
	int handleMouseClick(const SDL_Event& click) {
		std::cout << click.button.x << "\n";
		if (click.button.x > mscrollBackgroundRect.x && click.button.x < mscrollBackgroundRect.x + mscrollBackgroundRect.w
			&& click.button.y > mscrollBackgroundRect.y && click.button.y < mscrollBackgroundRect.y + mscrollBackgroundRect.h
		) 
		{
				if (click.button.y <mcurrYPos)
				{
					
					return -1;
				}
				if (click.button.y >mcurrYPos+mheight)
				{
					return 1;
				}
		}
		return 0;
	}
};