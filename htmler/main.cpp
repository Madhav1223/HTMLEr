#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<stack>
#include<chrono>
#include<thread>
#include "parser.h"
#include "cssparser.h"
#include "tokenizer.h"
#include "node.h" 
#include "rendertree.h" 
#include"window.h"
#define endl '\n'
using namespace std::chrono;

int main(int mainc, char* argv[]) {
	auto start = system_clock::now();

	std::ifstream file{ "index.html" };
	std::ifstream cssfile{ "style.css" };
	if (!file || !cssfile) {
		cout << "Cannot open file" << endl;
		exit(EXIT_FAILURE);
	}
	std::stringstream buffer;
	std::stringstream cssbuffer;
	buffer << file.rdbuf();
	cssbuffer << cssfile.rdbuf();

	Document* document = new Document;
	Parser parser{ document };
	parser.parse(buffer.str());
	CssParser cssparser{ };
	cssparser.parse(cssbuffer.str());


	auto end = system_clock::now();
	auto timetaken = (end - start);
	cout << endl;
	cout << endl;
	cout << "timetaken " << timetaken / 1ms << " ms" << endl;
	cout << "timetaken " << timetaken / 1s << " s" << endl;
	cout << endl;
	cout << endl;

	RenderTree* rendertree = new RenderTree;
	rendertree->createFromDom(document);


	rendertree->addRootStyle();
	rendertree->addStyle(cssparser);

	int scrollbarWidth = 20;
	Window window{ scrollbarWidth };
	RenderTree::setStatic(&window);
	int w{};
	window.getWindowSize(&w);
	rendertree->calculateLayout(w - scrollbarWidth);
	//decrease the size of w by width of scroll bar



	 end = system_clock::now();
	 timetaken = (end - start);
	cout << endl;
	cout << endl;
	cout << "timetaken " << timetaken / 1ms << " ms" << endl;
	cout << "timetaken " << timetaken / 1s << " s" << endl;
	cout << endl;
	cout << endl;

	window.setRootColor();
	window.eventloop(rendertree);
	//	SDL_Delay(100000);

	//auto hi = window.getFontSize(str2);
	//cout << "=======================================" << endl;
	//cout << "width " << hi.first << endl;
	//cout << "=======================================" << endl;
	delete rendertree;
	delete document;
	TTF_Quit();
	SDL_Quit();
	return 0;
}
