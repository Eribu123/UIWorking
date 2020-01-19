#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

/*
a couple of things to keep in mind when we add the next horizontal tab

1) you have to change the limit on the size of the key pressed, in the draw to method
2) you have to declare it new in the main
3) it needs to have a drawTo() method

*/

class HorizontalTabManager {

private:
	vector<BaseTabMenu*> baseTabs;
	Cities* cities;
	Items* items;
	Font font;
	vector<Texture> textures;
public:
	HorizontalTabManager(){}
	HorizontalTabManager(vector<BaseTabMenu*> baseTabs,Cities cities[],Font& font, vector<Texture> textures) {
		this->cities = cities;
		/*
		look into this line mybe 
		*/
		this->baseTabs = baseTabs;
	}
	void drawTo(int tabToOpen, RenderWindow& window) {
		//cout <<"tab: "<< tabToOpen<<endl;
		
		//cout <<"City UI "<< baseTabs[1] << endl;
		if (tabToOpen <= 1) {
			baseTabs[tabToOpen]->drawTo(window);
		}
	}
	void checkInput(Event event) {

	}
};

class HeadsUpDisplay {
private:
	Map* map;
	TextBox textbox;
	
public:
	HeadsUpDisplay() {}
	~HeadsUpDisplay() {}
	HeadsUpDisplay(Map* map, Font& font) {
		this->map = map;
		textbox = TextBox("", { 500,120 }, 50, font, Color::Green, Color::Blue, { 1000,0 });
		//textbox.
	}
	void drawTo(RenderWindow& window) {
		//string txt = "day:"+map.day+"month: ";
		//map->dayTime();
		textbox.setText("day: " + to_string(map->day) + " Month: " + to_string(map->month) + "\n year: " + to_string(map->year) + " hour: " + to_string(map->hour));
		textbox.drawTo(window);
	}

};

class UIManager {
private:
	//RenderWindow* window;
	Font font;
public:
	UIManager(){}
	~UIManager() {}

	HorizontalTabManager horizontalTabs;
	HeadsUpDisplay headsUpDisplay;
	UIManager(Font& font,vector<BaseTabMenu*> baseTabs,Cities* cities,vector<Texture> tabTextures,Map* map) {
		//this->window = &window;
		headsUpDisplay = HeadsUpDisplay(map, font);
		horizontalTabs = HorizontalTabManager(baseTabs, cities, font, tabTextures);
	}

	void drawHorizontalTabs(int input,RenderWindow& window) {
		horizontalTabs.drawTo(input,window);
	}

	void drawHeadsUpDisplay(RenderWindow& window) {
		headsUpDisplay.drawTo(window);
	}
};