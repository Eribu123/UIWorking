#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
using namespace sf;
using namespace std;


/*
IS DEPRECATED, USE INTERFACE TOOLS INSTEAD


*/

class CityUI;
template<typename T>
class Clickable {
public:
	void(T::* clickedWindow)(RenderWindow& window);
};

class Button {
private:
	sf::RectangleShape outlineShape;
	sf::Text text;

public:
	//void(*clicked)();
	static vector<Button> allButtons;

	void setClicked(void(*f)()) {
	}

	template<typename T>
	void setClicked(void(T::*c)(RenderWindow& window)) {
		clickedWindow.clickedWindow = c;
	}
	Button() {}
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor,Vector2f pos) {
		outlineShape.setSize(buttonSize);
		outlineShape.setFillColor(bgColor);

		text.setString(btnText);
		text.setFillColor(textColor);
		text.setCharacterSize(charSize);
		setPosition(pos);
		allButtons.push_back(*this);
	}
	void setText(string str) {
		text.setString(str);
	}
	void setFont(Font& font) {
		text.setFont(font);
	}
	void printGlobalPos() {
		cout << "3: " << " H " << outlineShape.getGlobalBounds().height << " L " << outlineShape.getGlobalBounds().left << " w " << outlineShape.getGlobalBounds().width << " T "
			<< outlineShape.getGlobalBounds().top << endl;
	}
	void setPosition(Vector2f pos) {
		cout <<"1: " << " H " << outlineShape.getGlobalBounds().height << " L " << outlineShape.getGlobalBounds().left << " w " << outlineShape.getGlobalBounds().width << " T "
			<< outlineShape.getGlobalBounds().top << endl;

		outlineShape.setPosition(pos);
		float xPos = (pos.x + outlineShape.getGlobalBounds().width / 4) - (text.getGlobalBounds().width / 2);
		float yPos = (pos.y + outlineShape.getGlobalBounds().height / 4) - (text.getGlobalBounds().height / 2);
		text.setPosition({ xPos,yPos });
		cout << "2: " << " H " << outlineShape.getGlobalBounds().height << " L " << outlineShape.getGlobalBounds().left << " w " << outlineShape.getGlobalBounds().width << " T "
			<< outlineShape.getGlobalBounds().top << endl;

	}
	void setSize(Vector2f v) {
		outlineShape.setSize(v);
	}
	void drawTo(RenderWindow& window) {
		window.draw(outlineShape);
		window.draw(text);
	}
	bool isMouseOver(RenderWindow& window) {
		//cout << " 0-click: " << outlineShape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		cout << " H " << outlineShape.getGlobalBounds().height << " L " << outlineShape.getGlobalBounds().left << " w " << outlineShape.getGlobalBounds().width << " T "
			<< outlineShape.getGlobalBounds().top<<endl;
		//cout << Mouse::getPosition(window).x<<", "<<Mouse::getPosition(window).y<<endl;
		return outlineShape.getGlobalBounds().contains((Vector2f)/*window.mapPixelToCoords*/(sf::Mouse::getPosition(window)));
	}
	void setFillColor(Color color) {
		outlineShape.setFillColor(color);
	}
};
vector<Button> Button::allButtons;

class InputTextBox {
private:
	Text text;
	std::ostringstream txt;
	bool isSelected=false, hasLimit=false;
	int limit=1;

public:
	InputTextBox() {

	}
	InputTextBox(int size, Color color, bool sel = true) {
		this->text.setCharacterSize(size);
		this->text.setFillColor(color);
		isSelected = sel;
		if (sel) {
			text.setString("_");
		}
		else {
			text.setString(" ");
		}
	}
	~InputTextBox(){}
	void inputLogic(int charTyped) {
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
			txt << static_cast<char>(charTyped);
		}
		else if (charTyped == DELETE_KEY) {
			if (txt.str().length() > 0) {
				deleteLastChar();
			}
		}
		text.setString(txt.str() + " ");

	}
	void setText(string input) {
		txt << input;
		text.setString(input);

	}
	void deleteLastChar() {
		string t = txt.str();
		string newTxt = "";
		for (int i = 0; i < t.length(); i++) {
			newTxt += t[i];
		}
		txt.str("");
		txt << newTxt;
		text.setString(txt.str());

	}
	void setFont(Font& font) {
		text.setFont(font);
	}
	void setPosition(Vector2f pos) {
		text.setPosition(pos);
	}
	void setLimit(bool isLimit) {
		hasLimit = isLimit;
	}
	void setLimit(bool ToF, int lim) {
		hasLimit = ToF;
		limit = lim - 1;
	}


	void setSelected(bool sel) {
		isSelected = sel;
		if (!sel) {
			string t = txt.str();
			string newTxt = "";
			for (int i = 0; i < t.length() - 1; i++) {
				newTxt += t[i];
			}
			text.setString(newTxt);
		}
	}
	string getText() {
		return text.getString();
	}
	void drawTo(RenderWindow& window) {
		window.draw(text);
	}
	void typedOn(Event input) {
		if (isSelected) {
			int charTyped = input.text.unicode;
			//cout << input.text.unicode;
			if (charTyped < 128) {
				if (hasLimit) {
					if (txt.str().length() <= limit) {
						inputLogic(charTyped);
					}
					else if (txt.str().length() > limit&& charTyped == DELETE_KEY) {
						deleteLastChar();
					}
				}
				else {
					inputLogic(charTyped);
				}
			}
		}
	}
	void parseInteger(Event input) {
		if (isSelected) {
			//cout << input.text.unicode;
			//if(input.text.unicode)
		}
	}
	InputTextBox(const InputTextBox& t) { txt << t.txt.rdbuf(); }

};

class ItemsPanel {
private:
	RectangleShape backgroundRect;
	RectangleShape icon;
	Text name,prices,amtOwned;
	InputTextBox input;
public:
	Button buyButton, sellButton;
	void drawto(RenderWindow& window,Font& font){
		setFont(font);
		window.draw(backgroundRect);
		window.draw(icon);
		window.draw(name);
		window.draw(prices);
		window.draw(amtOwned);
		input.drawTo(window);
		buyButton.drawTo(window);
		sellButton.drawTo(window);


	}
	ItemsPanel(){}
	//~ItemsPanel

	ItemsPanel(Items item) {
		icon = item.getIcon();
		backgroundRect = RectangleShape({ 25, 50 });
		backgroundRect.setFillColor({0,255,0,125});
		name.setString(item.getName());
		prices.setString("value: " + to_string(item.getValue()));
	}
	~ItemsPanel() {}

	void setPosition(Vector2f pos) {
		backgroundRect.setPosition(pos);
		name.setPosition(pos);
		prices.setPosition(pos);
		icon.setPosition(pos);
	}
	void setAmount(Event& event) {
		input.typedOn(event);
		//amtOwned = event;
		/*
		remember to switch to parse int later
		*/
	}
	void setFont(Font& font) {
		name.setFont(font);
		prices.setFont(font);
	}
	string test() {
		return "";
	}
};

