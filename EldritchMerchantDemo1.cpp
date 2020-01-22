// EldritchMerchantDemo1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "TestingHeader.h"
#include "UserInterfaceTools.h"
#include "HorizontalDropDownMenu.h"
#include "FinalUI.h"
#include <typeinfo>

/*

TODO: MAKE IT SO THAT THE SELECTED INPUTTEXTBOX IS THE ONLY ONE W/ SET SELECTED TRUE, PERHAPS JUST MODIFY WHAT IT MEANS TO BE SELECTED IN THE INPUTTEXTBOX CLASS
*/



Vector2f getMouseVeloctiy(Vector2i& prevPos) {
	Vector2f mouseVel;
	if (prevPos == Vector2i(0, 0)) {
		mouseVel = Vector2f(Mouse::getPosition());
		prevPos = Mouse::getPosition();
	} else {
		mouseVel = Vector2f(prevPos -= Mouse::getPosition());
		prevPos = Mouse::getPosition();
	}
	//we may want to use acceleration to prevent noise
	mouseVel = normalization(mouseVel);
	return mouseVel;
}

string getCities(Cities cities[], int numCities) {
	string txt = "";
	for (int i = 0; i < numCities; i++) {
		txt += cities[i].toString();
	}
	return txt;
}

void createRenderWindow(RenderWindow& window, Vector2f windowSize) {
	Vector2i centerWindow((VideoMode::getDesktopMode().width / 2) - 445, (VideoMode::getDesktopMode().height / 2) - 480);
	window.create(VideoMode(windowSize.x, windowSize.y), "SFML Project");
	window.setPosition(centerWindow);
	window.setKeyRepeatEnabled(true);
}

int main() {
	/*sets up window*/
	Vector2f windowSize({ 1800,1080 });
	RenderWindow window;
	createRenderWindow(window, windowSize);
	Font font;

	/*generates font*/
	if (!font.loadFromFile("C:/Users/johnd/source/repos/EldritchMerchantDemo1/arial/arial.ttf")) {
		cerr << "ERR";
	}
	/*creates items and cities*/
	Items items[] = { Items("Beer",1),Items("Holy Dildos",2),Items("Sword",99),Items("Cocaine",22) };
	map<Items, float> m;
	for (int j = 0; j < sizeof(items) / sizeof(items[0]); j++) {
		m.insert({ items[j], static_cast <float> (rand()) });
	}
	Cities cities[] = { Cities("London", m , Vector2f(700,400)), Cities("NYC", m,Vector2f(700,450)), Cities("Gotham",m, Vector2f(700,500)), Cities("Pozan",m, Vector2f(700,800)) };
	for (int i = 0; i < Cities::numCities; i++) {
		cities[i].randomizeItems(items);
	}

	/*sets up player*/
	map<Items, float> plrItems;
	Player player(items, cities[0]);

	//BaseTabMenu* tabs[] = { { new InventoryUI(items,font) }, { new CityUI(cities, font) } };
	InventoryUI iUI = InventoryUI(items, font, player.inventory);
	vector<BaseTabMenu*> tabs = { &iUI, new CityUI(cities, font,&player) };

	/* generates map */
	Texture MapTexture;
	if (!MapTexture.loadFromFile("MAP(B&B).jpg")) {
		cerr << "err" << endl;
	}
	Map map(MapTexture, windowSize, &player);

	/* miscanlious stuff*/
	bool panScreen = false;
	/*for settings later  */
	float panningSpeed = 1, prevMouseWheelDelta;
	// create a view with the rectangular area of the 2D world to show
	sf::View view1(sf::FloatRect(0, 0, 1800.f, 1080.f));
	Vector2i prevMousePos;
	Vector2f mouseVelocity;


	UIManager ui(/*window*/ font, tabs, cities, vector<Texture>(), &map);
	//CityUI c = CityUI(cities, font,player);
	//BaseTabMenu* bt = &c;

	InputTextBox ITBTesting = InputTextBox("#", 15, font, { 500,500 }, Color::Black, { 500,0 }, 15, Color::White);
	InputTextBox ITBTesting1 = InputTextBox("#", 15, font, { 500,500 }, Color::Black, { 10,500 }, 15, Color::White);

	ITBTesting.setFont(font);
	int input = -1;
	while (window.isOpen()) {
		Event event;

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			ITBTesting.setSelected(true);
		} else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			ITBTesting.setSelected(false);
		}
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			} else if (event.type == Event::TextEntered) {
				//ITBTesting.typedOn(event);
				cout <<(Button::selectedButton!=nullptr)<<endl;
				if (Button::selectedButton != nullptr) {
					cout << typeid((*Button::selectedButton)).name() << endl;
					//cout << (*Button::selectedButton).getClassType()<< endl;
					cout << (Button::selectedButton)->getClassType() << endl;

					cout << Button::selectedButton << endl;
					cout << typeid(static_cast<InputTextBox*>(Button::selectedButton)).name() << endl;
				}
				cout << typeid(InputTextBox).name()<<endl;
				if (typeid(Button::selectedButton) == typeid(InputTextBox)) {
					InputTextBox* itBox = dynamic_cast<InputTextBox*>( Button::selectedButton);
					cout << itBox << endl;
					cout <<"selected button: "<< &Button::selectedButton << endl;

					itBox->typedOn(event);
				}
			} else if (event.type == Event::KeyPressed) {
				if (event.text.unicode > 25 && event.text.unicode < 36) {
					cout << (int)event.text.unicode;
					if (input == ((int)event.text.unicode - 26))
						input = -1;
					else
						input = event.text.unicode - 26;
				}
			} else if (event.type == Event::MouseMoved) {
				if (panScreen) {

					//cout << mouseVelocity.x << ", " << mouseVelocity.y << endl;
					mouseVelocity = getMouseVeloctiy(prevMousePos);
					view1.move(mouseVelocity * panningSpeed);
				}
				if (Keyboard::isKeyPressed(sf::Keyboard::LAlt) || Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
					//prevMouseWheelDelta = event.mouseWheel.delta;
					//cout << event.mouseWheel.delta << endl;
					//cout << view1.getSize().y<<", "<<view1.getSize().x << endl;
					//cout << getMouseVeloctiy(prevMousePos).y << ", " << view1.getSize().x << endl;
					//if (view1.getSize().y >= 1000 && view1.getSize().y <= 1080) {
					float zoomAmt = 1 + getMouseVeloctiy(prevMousePos).y * .1f;
					if (view1.getSize().y <= 1000 && zoomAmt < 1)
						zoomAmt = 1;
					else if (view1.getSize().y >= 1080 && zoomAmt > 1)
						zoomAmt = 1;
					view1.zoom(zoomAmt);
					//}
				}

				//we could use this to highlight the current button that the mouse is over
			} else if (event.type == Event::MouseButtonPressed) {
				//Button::allButtons[0]->printGlobalPos();
				for(int i=0;i<tabs[1]->buttons.size();i++){
					if (tabs[1]->buttons[i].isMouseOver(window)) {
						tabs[1]->pressed(i);
					}
				}
				for (int i = 0; i < tabs[0]->buttons.size(); i++) {
					if (tabs[0]->buttons[i].isMouseOver(window)) {
						tabs[0]->pressed(i);
					}
				}
				panScreen = true;
			}
			//else if (event.type == Event::MouseButtonReleased) {
			panScreen = false;
			//}
		}
		//cout << textBox1.getText();
		window.clear();
		map.drawTo(window);
		for (int i = 0; i < Cities::numCities; i++) {
			cities[i].drawTo(window);
		}
		if (player.targetPos != player.curPos) {
			player.travel();
			cout << "please work" << endl;
		}
		if (map.day % 28 == 0) {
			for (int i = 0; i < Cities::numCities; i++) {
				cities[i].randomizeItems(items);
				for (int j = 0; j < sizeof(items) / sizeof(items[0]); j++) {
					cities[i].addToHistory(items[j], cities[i].getItemRatio(items[j]));
				}
			}
		}
		player.drawTo(window);
		map.dayTime();
		//bt->drawTo(window);
		ui.drawHeadsUpDisplay(window);
		if (input >= 0) {
			//cout << input;
			ui.drawHorizontalTabs(input, window);

		}
		//ITBTesting.drawTo(window);
		//ITBTesting1.drawTo(window);

		window.display();
		window.setView(view1);

	}
}
