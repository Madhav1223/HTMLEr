#pragma once
#include"states.h"

class Token {
protected:
	TokenType type;
public:
	Token(TokenType t) :type{ t } {}
};

class Comment : public Token {
private:
	std::string mcomment;
public:
	Comment() :Token{ TOK_COMMENT } {}
	void add(char c) {
		mcomment += c;
	}
};

class Character : public Token {
	std::string mchar;
public:
	Character() :Token{ CHARACTER } {}
	void add(char c) {
		mchar += c;
	}
};

class EndTag :public Token {
private:
	TagName tagName;
public:
	EndTag(const std::string& tn) :Token{ END_TAG }, tagName{ getTagNameAsEnum(tn) }{
	}
};

class Tag :public Token {
private:
	TagName tagName;
public:
	Tag(const std::string& tn) :Token{ END_TAG }, tagName{ getTagNameAsEnum(tn) }{
	}

};





