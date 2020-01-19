#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
using namespace sf;
using namespace std;

class Panel{
protected:
	RectangleShape outlineShape;
public:
	Panel(sf::Vector2f buttonSize, sf::Color bgColor, Vector2f pos) {
		outlineShape.setSize(buttonSize);
		outlineShape.setFillColor(bgColor);
		outlineShape.setPosition(pos);
	}
	virtual void setPosition(Vector2f pos) {
		outlineShape.setPosition(pos);
	}
	virtual void setPosition(Vector2f pos,bool left){
		outlineShape.setPosition(pos);
		cout << "calling left";
	}
	void setColor(Color color) {
		outlineShape.setFillColor(color);
	}
	Vector2f getPosition(){
		return outlineShape.getPosition();
	}
	virtual void drawTo(RenderWindow& window) {
		window.draw(outlineShape);
	}
	void setSize(Vector2f v) {
		outlineShape.setSize(v);
	}
	Vector2f getSize() {
		return outlineShape.getSize();
	}
	
	Panel(){}
	~Panel(){}

};
class TextBox :public Panel {
protected:
	Text text;
public:
	TextBox(string s, Vector2f buttonSize, int characterSize,Font& font, sf::Color bgColor, Color txtColor, Vector2f pos) : Panel(buttonSize, bgColor, pos) {
		text.setString(s);
		text.setCharacterSize(characterSize);
		text.setFont(font);
		setPosition(pos);
		text.setFillColor(txtColor);
	}
	TextBox(string s, Vector2f buttonSize, int characterSize,  sf::Color bgColor, Color txtColor, Vector2f pos) : Panel(buttonSize, bgColor, pos) {
		text.setString(s);
		text.setCharacterSize(characterSize);
		setPosition(pos);
		text.setFillColor(txtColor);
	}
	void setFont(Font& font) {
		text.setFont(font);
	}
	void setPosition(Vector2f pos,bool left =false) override{
		Panel::setPosition(pos);
		if (!left) {
			float yPos = (pos.y + outlineShape.getGlobalBounds().height / 4);
			//float xPos = (pos.x + outlineShape.getGlobalBounds().width / 2) - (text.getGlobalBounds().width/2);
			float xPos = (pos.x + outlineShape.getGlobalBounds().width / 4);

			text.setPosition({ xPos,yPos });
		}
		else {
			float xPos = (pos.x +3);
			float yPos = (pos.y + outlineShape.getGlobalBounds().height / 4)+-3;
			text.setPosition({ xPos,yPos });

		}
	}
	virtual void setText(string s) {
		text.setString(s);
	}
	float getSizeOfText() {
		return text.getCharacterSize()* text.getString().getSize();
	}
	string getText() {
		return text.getString();
	}
	void drawTo(RenderWindow& window) override {
		Panel::drawTo(window);
		window.draw(text);
	}
	bool isMouseOver(RenderWindow& window) {
		return outlineShape.getGlobalBounds().contains((Vector2f)(Mouse::getPosition(window)));
	}
	TextBox(){}
	~TextBox(){}
};
class Button : public TextBox {
public:
	//static vector<Button*> allButtons1;
	//static vector<Button&> allButtons2;
	static vector<Button> allButtons;
	bool isSelected;
	void (*clicked)(RenderWindow&)=nullptr;

	Button(){}
	~Button(){}
	void setFont(Font& font) {
		text.setFont(font);
	}
	Button(string s, Vector2f buttonSize, int characterSize, Font& font, sf::Color bgColor, Color txtColor, Vector2f pos) :TextBox(s, buttonSize, characterSize, font, bgColor,txtColor,pos) {
		cout << "clicked: " <<clicked<<endl;

		//allButtons1.push_back(&*this);
		allButtons.push_back(*this);

	}
	Button(string s, Vector2f buttonSize, int characterSize, sf::Color bgColor, Color txtColor, Vector2f pos) :TextBox(s, buttonSize, characterSize, bgColor, txtColor, pos) {
		//clicked = nullptr;
		//allButtons.push_back(this);
	}
	void printGlobalPos() {
		cout << "3: " << " H " << outlineShape.getGlobalBounds().height << " L " << outlineShape.getGlobalBounds().left << " w " << outlineShape.getGlobalBounds().width << " T "
			<< outlineShape.getGlobalBounds().top << endl;
	}

};
vector<Button> Button::allButtons;
//vector<Button*> Button::allButtons1;
//vector<Button&> Button::allButtons2;


class InputTextBox :public Button {
private:
	std::ostringstream txt;
	bool hasLimit=false;
	int limit=1;
	static bool selectedATextBox;

public:
	InputTextBox() {}
	InputTextBox(string s, int characterSize, Font& font, Vector2f buttonSize, sf::Color bgColor, Vector2f pos, int size, Color color, bool sel = false) :Button(s, buttonSize, characterSize, font, bgColor, color, pos) {
		this->text.setCharacterSize(size);
		this->text.setFillColor(color);
		isSelected = sel;
		text.setFont(font);
		if (sel) {
			text.setString("_");
		}
		else {
			text.setString(" ");
		}
	}
	InputTextBox(string s, int characterSize, Vector2f buttonSize, sf::Color bgColor, Vector2f pos, int size, Color color, bool sel = false) :Button(s, buttonSize, characterSize, bgColor, color, pos) {
		this->text.setCharacterSize(size);
		this->text.setFillColor(color);
		isSelected = sel;
		if (sel) {
			text.setString("_");
		} else {
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
	void setText(string input) override {
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
			for (int i = 0; i < t.length(); i++) {
				newTxt += t[i];
			}
			text.setString(newTxt);
		}
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
bool InputTextBox::selectedATextBox;

class ItemsPanel {
private:
	RectangleShape backgroundRect;
	RectangleShape icon;
	TextBox name,prices,amtOwned;
	InputTextBox* input;
	Button buyButton, sellButton;

public:
	void drawto(RenderWindow& window,Font& font){
		setFont(font);
		window.draw(backgroundRect);
		window.draw(icon);
		input->drawTo(window);
		buyButton.drawTo(window);
		sellButton.drawTo(window);
		name.drawTo(window);
		prices.drawTo(window);
		amtOwned.drawTo(window);
	}
	InputTextBox* getInputBox() {
		return input;
	}
	ItemsPanel(){}
	//~ItemsPanel

	ItemsPanel(Items item,Vector2f pos,map<Items,int> inventory) {
		buyButton = Button("Buy", { 50,50 }, 15, Color::Magenta, Color::Black, {pos.x+15,pos.y+150});
		buyButton.setText("buy");
		sellButton = Button("sell", { 50,50 }, 15, Color::Magenta, Color::Black, {pos.x+150,pos.y+150});
		//float si
		
		cout << item.getName()<<endl;
		icon = item.getIcon();
		backgroundRect = RectangleShape({ 300, 200 });
		backgroundRect.setFillColor({0,255,0,125});
		cout << item.getName() << endl << endl;
		/*
		position is set in the next abstraction down
		*/
		Vector2f textBoxSize = { 140,30 };

		icon = RectangleShape({ 50,60 });
		icon.setFillColor(Color::Black);


		name = TextBox(item.getName(), textBoxSize, 15, Color::Blue, Color::Black, { 0,0 });
		cout << (item) << endl;

		amtOwned = TextBox("amt owned: "+to_string(inventory.at(item)) , textBoxSize, 15, Color::Blue, Color::Black, { 0,0 });
		
		//name.setPosition(pos);
		prices = TextBox(("value: " + to_string(item.getValue())), textBoxSize, 15, Color::Blue, Color::Black, { 0,0 });
		input = new InputTextBox("#", 15, { 50,50 }, Color::Black, Vector2f(pos.x + 75,pos.y+150 ), 15, Color::White);
	
		//input->clicked = input->setSelected(true);
	}
	~ItemsPanel() {}

	void setPosition(Vector2f pos) {
		backgroundRect.setPosition(pos);
		name.setPosition({pos.x	+10,pos.y+10},true);
		amtOwned.setPosition({ (name.getPosition().x+100),name.getPosition().y }, true);
		
		icon.setPosition(pos.x+10,pos.y+75);

		
		prices.setPosition({ (name.getPosition().x+100),name.getPosition().y +75}, true);

	}
	void setAmount(Event& event) {
		input->typedOn(event);
		//amtOwned = event;
		/*
		remember to switch to parse int later
		*/
	}
	void setFont(Font& font) {
		name.setFont(font);
		prices.setFont(font);
		amtOwned.setFont(font);
		buyButton.setFont(font);
		sellButton.setFont(font);
		input->setFont(font);
	}
	
};

class Bar {
public:
	Bar();
	~Bar();

private:

};

