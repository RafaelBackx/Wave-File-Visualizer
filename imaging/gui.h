#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>

namespace gui
{
	class Node
	{
	public:
		virtual void onClick(sf::Vector2f mouse) = 0;
		virtual void onHover(sf::Vector2f mouse) = 0;
		//virtual void onLeave(sf::Vector2f mouse) = 0;
		virtual ~Node() {}
	};

	class Button : public Node
	{
	private:
		sf::RectangleShape buttonShape;
		sf::Text text;
		sf::Font font;
		sf::Color color;
		sf::Color hoverColor;
		int textWidth;
		int characterSize = 16;
	public:
		Button(int buttonWidth, int buttonHeight, std::string text) : buttonShape(sf::Vector2f(buttonWidth, buttonHeight)), color(sf::Color::Black), hoverColor(sf::Color(64,63,60))
		{
			if (!this->font.loadFromFile("sprites\\arial.ttf"))
			{
				//If you dont have roboto, i hope you have arial, isn't this a windows default or something ?
				this->font.loadFromFile("sprites\\arial.ttf");
			}
			this->buttonShape.setFillColor(sf::Color::Green);
			this->text.setFont(this->font);
			this->text.setString(text);
			this->text.setCharacterSize(this->characterSize);
			this->text.setFillColor(sf::Color::Black);
			this->textWidth = text.length() * this->characterSize;
		}

		int getWidth() { return this->buttonShape.getSize().x; }
		int getHeight() { return this->buttonShape.getSize().y; }
		sf::Vector2f getSize() { return this->buttonShape.getSize(); }
		std::string getText() { return this->text.getString().toAnsiString(); }
		sf::RectangleShape& getShape() { return this->buttonShape; }

		void setSize(sf::Vector2f size) { this->buttonShape.setSize(size); }
		void setText(std::string text) { this->text.setString(text); }
		void setCharacterSize(int charactersize) { this->text.setCharacterSize(characterSize); }
		void setPosition(float width, float height);
		void setPosition(sf::Vector2f position);

		//void onClick(void(*action)()) { action(); }
		//void onClick(void(*action)(Button& button), Button& button) { action(button); }
		void onClick(sf::Vector2f mouse) override;
		void onHover(sf::Vector2f mouse) override;
		void draw(sf::RenderWindow& window);
	};

	class Screen
	{
	private:
	public:
		std::vector<Node*> nodes;
		Screen() : nodes(0){}
		void addNode(Node* node) { this->nodes.emplace_back(node); }
		//void removeNode(Node node); //TODO find iterator of object and remove it by iterator
		void updateNodesOnClick(sf::Vector2f mouse) { for (Node* n : this->nodes) { n->onClick(mouse); } }
		void updateNodesOnHover(sf::Vector2f mouse) { for (Node* n : this->nodes) { n->onHover(mouse); } }
		virtual ~Screen(){}
	};
}
#endif // !GUI_H