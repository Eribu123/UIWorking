#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include <time.h>
#include <unordered_map>
//#include<sstream>
using namespace std;
using namespace sf;
static Vector2f normalization(Vector2f tarVec) {
	Vector2f v = { tarVec.x * tarVec.x , tarVec.y * tarVec.y };
	float length = sqrt(v.x + v.y);
	if(length>0)
		v = { tarVec.x / length, (tarVec.y / length)};
	return v;
}
class Items {
private:
	int value;
	string name;
	RectangleShape icon;

public:
	Items() {}
	Items(string name, int value) {
		this->name = name;
		this->value= value;
	}
	~Items() {}
	int getValue() {
		return this->value;
	}
	string getName() {
		return name;
	}
	friend ostream& operator<<(ostream& strm, const Items& a) {
		return strm << "(Item : " << a.name << "value: " << a.value << ")";
	}
	friend bool operator==(const Items& a,const Items& b) {
		return	(a.name == b.name);
	}
	friend bool operator<(const Items& a, const Items& b) {
		return a.value < b.value;
	}
	friend ostream& operator<<(ostream& strm, const vector<Items>& a) {
		string output = "";
		for (unsigned int i = 0; i < a.size(); i++) {
			output += "(Item : ";
			output += a[i].name;
			output += " value: ";
			output += to_string((a[i].value));
			output+= ")";

			return strm << output;
		}
	}
	string toString() {
		return "name: " +name+" Value: "+to_string(value);
	}
	RectangleShape getIcon() {
		return icon;
	}
	
};
class Cities;

class Player {
private:
	RectangleShape icon;

public:
	int coffer;
	//items, to the amount the player have of that items
	map<Items, int> inventory;

	Cities* curCity;
	Vector2f targetPos, curPos;
	Player() {}
	Player(Items allItems[], Cities& london) {
		cout << "size " <<sizeof(allItems) << endl;
		for (int i = 0; i < sizeof(allItems); i++) {
			cout << "building inventory" << endl;
			cout << allItems[i].getName() << endl;
			inventory.insert({ allItems[i],99 });
		}
		coffer = 9999;
		curCity = &london;
		icon.setSize({ 15,15 });
		icon.setFillColor(Color::Red);
		icon.setPosition({ 700,400 });
	}
	Player(Items* items, Cities& london, Texture& iconTexture) {
		for (int i = 0; i < sizeof(items); i++) {
			cout << "building inventory"<<endl;
			cout << items[i].getName() << endl;

			this->inventory.insert({ items[i],99 });

		}
		coffer = 9999;
		curCity = &london;
		this->icon = icon;
		icon.setSize({ 15,15 });
		//icon.setFillColor(Color::Red);
		icon.setPosition({ 350,100 });
		icon.setTexture(&iconTexture);
	}
	void travel() {
		curPos = icon.getPosition();
		Vector2f dir = (targetPos - curPos);
		Vector2f ndir = normalization(dir);
		float speed = 1;
		cout << "ndir: " << ndir.x << ", " << ndir.y << endl;

		icon.move(ndir * speed);
	}

	void drawTo(RenderWindow& window) {
		window.draw(icon);
	}
	void outPutItems() {
		for (pair<Items, float> element : inventory) {
			cout << element.first.getName() << " amt " << element.second;
		}
	}
};

class Cities {
private:
	string name;
	Items itmProduced;
	Vector2f location;
	//items, supply-demand Ratio;
	RectangleShape icon;
	map<Items,float> allItemsInThisCity; 
	//item, history of supply demandRatio
	map<Items, vector<float>>itemHistory;

public:
	static int numCities;
	int id = 0;
	int getItemValue(Items item,float ratio ) {
		return item.getValue() * ratio;
	}
	Cities(string name, map<Items, float> allItemsInThisCity,Vector2f location) {
		id = numCities;
		numCities++;
		this->name=name;
		this->location = location;
		this->allItemsInThisCity = allItemsInThisCity;
		initilizeHistory();
		icon.setSize({ 35,35 });
		icon.setPosition(location);
		icon.setFillColor(Color::Green);
	}
	Cities(){}
	~Cities() {}
	string toString() {
		string txt = "";
		txt += name;
		txt += ", location: (" + to_string(location.x)+" , "+to_string(location.y) + "), Items: \n";
		map<Items, float>::iterator it = allItemsInThisCity.begin();
		for (pair<Items, float>element : allItemsInThisCity) {
			float val = (element.first.getValue())*(element.second);
			txt += "\t"+element.first.getName() + " value: " + to_string(val) + " \n";
		}
		return txt;
	}
	Vector2f getLocation() {
		return location;
	}
	void randomizeItems(Items items[]) {
		for (pair<Items, float> element : allItemsInThisCity) {
			allItemsInThisCity.at(element.first) = static_cast <float> (rand());
		}
	}
	
	float getItemRatio(Items item) {
		return allItemsInThisCity.at(item);
	}
	void initilizeHistory() {
		for (pair<Items, float> element : allItemsInThisCity) {
			vector<float> f;
			f.push_back(element.second);
			cout << element.first;
			this->itemHistory.insert({ element.first, f });
		}
	}
	void addToHistory(Items item,float newRatio) {
		itemHistory.at(item).push_back(newRatio);
	}
	//void add
	void sell(Items* item, Player* player,float amt) {
		player->coffer += item->getValue();
		//remove(player->items,)
		//basically remove returns an empty array that contains everything but the speicific value
		player->outPutItems();
		//player->items.erase(remove(player->items.begin(), player->items.end(), *item), player->items.end());
		player->inventory.at(*item) = player->inventory.at(*item) - amt;
		player->outPutItems();

	}
	void buy(Items* item,Player* player,float amt) {
		player->coffer -= item->getValue();
		player->outPutItems();
		player->inventory.at(*item) = player->inventory.at(*item) + amt;
		player->outPutItems();
	}
	string getName() {
		return name;
	}
	void drawTo(RenderWindow& window) {
		window.draw(icon);
	}
};
int Cities::numCities = 0;

class Map {
public:
	Image img;
	Texture texture;
	RectangleShape rect;
	vector<Color> colorsInMap;
	const Uint8* pixels;
	int day=1, week=0, month=1, year=1700;
	Player* player;
	float hour, timeModifer=10;

	bool sunDown() {
		return hour >= 19&&hour<=5;
	}
	bool night() {
		return hour > 21&&hour<=5;
	}
	Map(){}
	Map(Texture& texture,Vector2f size,Player* player) {
		this->texture;
		img = texture.copyToImage();
		pixels = img.getPixelsPtr();
		this->player = player;
		rect.setSize(size);
		rect.setTexture(&texture);/*
		const int mapSize = size.x * size.y;
		for (int i = 0; i < mapSize; i++) {
			colorsInMap.push_back(Color((img.getPixelsPtr()[i * 4]), (img.getPixelsPtr()[i * 4 + 1]), (img.getPixelsPtr()[i * 4 + 2]), ( img.getPixelsPtr()[i * 4 + 3])));
		}*/
	}
	void uintToArray(){}
	~Map(){}
	void drawTo(RenderWindow& window) {
		window.draw(rect);
	}
	void dayTime() {
		if (month % 12== 0) {
			year++;
			month = 1;
		}
		else if (day % 30 == 0) {
			month++;
			day = 1;
		}
		else if (day % 7 == 0)
			week++;
		if (hour >= 24) {
			day++;
			hour = 0;
		}
		hour += timeModifer;

	}
};
