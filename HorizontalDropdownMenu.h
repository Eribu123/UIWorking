#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class  BaseTabMenu {
protected:
	CircleShape icon;
public:
	vector<Button> buttons;
	RectangleShape panel;
	static BaseTabMenu* openMenu;
	virtual void pressed(int index) {
		Button::selectedButton = &buttons[index];
	}
	//delete if becomes useless
	bool getIsOpen() {
		return (openMenu == this);
	}
	//BaseTabMenu()
	BaseTabMenu() {
		panel = RectangleShape({ 500,500 });
		panel.setFillColor(Color(0, 0, 0, 125));
		icon = CircleShape({ 25 });
		//cout << endl << "called from base tab" << endl;
	}
	BaseTabMenu(Texture* texture) {
		panel = RectangleShape({ 500,500 });
		panel.setFillColor(Color(0, 0, 0, 125));
		icon.setTexture(texture);
		icon = CircleShape({ 25 });

	}
	void drawIcon(RenderWindow& window) {
		window.draw(icon);
	}
	virtual void drawTo(RenderWindow& window) {
		//cout << "x";
		window.draw(panel);
	}
};
BaseTabMenu* BaseTabMenu::openMenu = nullptr;

class InventoryUI : public BaseTabMenu {
private:
	//Items* items;
	map<Items, int>inventory;
	Items* items;

	vector<ItemsPanel> itemPanels;
	Font font;
public:
	InventoryUI() {

	}
	~InventoryUI() {}
	InventoryUI(Items* items, Texture* t) :BaseTabMenu(t) {
		this->items = items;
		//this->inventory = inventory;
	}
	InventoryUI(Items* items, Font& font, map<Items, int> inventory) {
		this->items = items;
		this->font = font;
		this->inventory = inventory;

	}
	vector<ItemsPanel> getItemPanels() {
		return itemPanels;
	}
	void generateGrid() {
		int sizeOfRow = sqrt(sizeof(items));
		for (int i = 0; i < sizeOfRow; i++) {
			for (int j = 0; j < sizeOfRow; j++) {
				//cout <<" " << (i * j + j )<<" ";
				InputTextBox ipt = InputTextBox();
				Vector2f pos = { static_cast<float>(i * 310),static_cast<float>(j * 210) };
				itemPanels.push_back({ items[i * sizeOfRow + j] , pos ,inventory });
				buttons.push_back(*(dynamic_cast<Button*>( (itemPanels[i * sizeOfRow + j].getInputBox()))));
				buttons.push_back(itemPanels[i * sizeOfRow + j].getBuyButton());
				buttons.push_back(itemPanels[i * sizeOfRow + j].getSellButton());

				itemPanels[i * sizeOfRow + j].setPosition(pos);

			}
		}
	}
	void pressed(int index) override{
		BaseTabMenu::pressed(index);

	}
	void drawTo(RenderWindow& window) override {
		if (itemPanels.size() == 0)
			generateGrid();
		window.draw(panel);
		for (int i = 0; i < itemPanels.size(); i++) {
			itemPanels[i].drawto(window, font);
		}
	}

};

class CityUI :public BaseTabMenu {
private:
	static	Cities* cities;
	static	Player* player;

public:
	void(*test)(RenderWindow& window);
	TextBox cityInfoPanel;
	CityUI() {}
	CityUI(Cities cities[], Font& font, Player* player) {
		cout << "this: " << this;

		this->cities = cities;
		CityUI::player = player;
		//cout << "panel size1 " << panel.getSize().x << ", " << panel.getSize().y << endl;
		for (unsigned int i = 0; i < Cities::numCities; i++) {
			Button b = { cities[i].getName(), { 250,50 }, 20, font, ((i % 2) == 0) ? Color::Red : Color::Blue, Color::White,{0,static_cast<float>(i * 100) + 1} };
			CityUI::buttons.push_back(b);
			//Button::allButtons2.push_back(b);
			cout << &b << endl;
			//cout << buttons[i] << endl;
			//Button::allButtons[i]->printGlobalPos();

		/*	cout << &buttons[i] << endl;
			buttons[i]->printGlobalPos();
			Button::allButtons[i]->printGlobalPos();
			cout << "B";*/
		}
		//test = buttons[0]->clicked;
		//cout << "clicked2: " << buttons[0]->clicked << endl;
		cityInfoPanel = TextBox("", { 30, 30 }, 15, font, Color::Red, Color::Black, { 500,500 });
		cout << "clicked3: " << test << endl;

	}
	CityUI(Cities cities[], Font& font, Texture* t) :BaseTabMenu(t) {
	}
	void pressed(int index) override {
		BaseTabMenu::pressed(index);
		player->targetPos = cities[index].getLocation();
	}
	void generateUI(RenderWindow& window) {
		for (unsigned int i = 0; i < sizeof(cities); i++) {
			if (buttons[i].isMouseOver(window)) {
				cityInfoPanel.setText(cities[i].toString());
				cityInfoPanel.drawTo(window);
				return;
			}
		}
	}
	void drawTo(RenderWindow& window) override {

		window.draw(BaseTabMenu::panel);
		for (unsigned int i = 0; i < Cities::numCities; i++) {
			buttons[i].drawTo(window);
		}
	}
	~CityUI() {}
};
Player* CityUI::player;
Cities* CityUI::cities;

//void(CityUI::* clickedCityButton) CityUI::clickedCityButton;

class HorseUI :BaseTabMenu {

};
class CharacterUI :BaseTabMenu {

};
class CompanionsUI :BaseTabMenu {

};
class JobLogUI :BaseTabMenu {

};
class LettersUI :BaseTabMenu {

};
class newsPaperUI :BaseTabMenu {

};