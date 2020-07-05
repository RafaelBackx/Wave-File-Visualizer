#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.hpp>
#include <variant>
#include <functional>
#include <iostream>

namespace gui
{
	class Node
	{
	public:
		virtual void onClick(sf::Vector2f mouse) = 0;
		virtual void onHover(sf::Vector2f mouse) = 0;
		virtual void onDrag(sf::Vector2f mouse) = 0;
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
		sf::Texture texture;
		std::variant<std::function<void()>,std::function<void(Button& btn)>> functions;
		int characterSize = 16;
		bool fitTextToSize = true;
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
			this->setCharacterSize(this->characterSize);
			this->text.setFillColor(sf::Color::White);	
		}

		const int getWidth() const { return this->buttonShape.getSize().x; }
		const int getHeight() const { return this->buttonShape.getSize().y; }
		const sf::Vector2f getSize() const { return this->buttonShape.getSize(); }
		const std::string getText() const { return this->text.getString().toAnsiString(); }
		sf::RectangleShape& getShape() { return this->buttonShape; }
		const bool getFitTextToSize() { return this->fitTextToSize; }
		const sf::Color getColor() { return this->color; }
		const sf::Color getHoverColor() { return this->hoverColor; }

		void setSize(sf::Vector2f size) { this->buttonShape.setSize(size); }
		void setText(std::string text) { this->text.setString(text); }
		void setCharacterSize(int charactersize) 
		{
			if (this->text.getLocalBounds().width>this->buttonShape.getLocalBounds().width)
			{
				auto bounds = this->text.getLocalBounds();
				std::cout << bounds.width << std::endl;
				auto size = this->buttonShape.getSize();
				unsigned int newSize = (size.x * size.y) / bounds.width;
				this->text.setCharacterSize(newSize);
			}
			else
			{
				this->text.setCharacterSize(characterSize);
			}
		}
		void setPosition(float width, float height);
		void setPosition(sf::Vector2f position);
		void setOnClick(std::function<void()>func) { this->functions = func; }
		void setOnClick(std::function<void(Button& btn)>func) { this->functions = func; }
		void setFitTextToSize(bool fit) { this->fitTextToSize = fit; }
		void setColor(sf::Color c) { this->color = c; this->buttonShape.setFillColor(c); }
		void setHoverColor(sf::Color c) { this->hoverColor = c; }
		void setTexture(sf::Texture texture) { this->texture = texture; this->texture.setSmooth(true); this->buttonShape.setTexture(&this->texture); }
		void setTexture(std::string pathToTexture)
		{
			if (!this->texture.loadFromFile(pathToTexture))
			{
				std::cout << "Could not load " << pathToTexture << std::endl;
			}
			else
			{
				this->texture.setSmooth(true);
				this->buttonShape.setTexture(&this->texture);
			}
		}
		void onClick(sf::Vector2f mouse) override;
		void onHover(sf::Vector2f mouse) override;
		void onDrag(sf::Vector2f mouse) override;
		void draw(sf::RenderWindow& window);
		virtual ~Button(){}
	private:
		void useClickFunction();
	};

	class Slider : public Node
	{
	private:
		sf::RectangleShape outerRect;
		sf::RectangleShape innerRect;
		sf::Color outerRectColor;
		sf::Color innerRectColor;
		sf::CircleShape circle;
		float value;
		float minValue, maxValue;
		std::variant<std::function<void()>> dragFunctions;
		std::variant<std::function<void()>> clickFunctions;
	public:
		/*Slider(int lineWidth, int lineHeight, float circleRadius) : Slider(lineWidth,lineHeight,circleRadius,0){}*/

		Slider(int lineWidth, int lineHeight,float circleRadius): outerRect(sf::Vector2f(lineWidth,lineHeight)), innerRect(sf::Vector2f(0,lineHeight)),value(0.0f), minValue(0.0f),maxValue(100.0f)
		{
			this->circle.setRadius(circleRadius);
			auto bounds = this->circle.getLocalBounds();
			this->circle.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		}

		const float getCircleRadius()const { return this->circle.getRadius(); }
		const int getLineLength()const { return this->outerRect.getSize().x; }
		const int getLineHeight()const { return this->outerRect.getSize().y; }
		const float getValue() const { return this->value; }
		const sf::CircleShape getCircle() const { return this->circle; }
		sf::RectangleShape& getRectangle() { return this->outerRect; }
		const float getWidth() const { return this->outerRect.getLocalBounds().width; }
		const float getHeight() const { return this->circle.getLocalBounds().height; }
		const float getMinValue() const { return this->minValue; }
		const float getMaxValue() const { return this->maxValue; }
		const sf::Color getInnerRectColor() const { return this->innerRectColor; }
		const sf::Color getOuterRectColor() const { return this->outerRectColor; }

		

		void setMinValue(float minValue) { this->minValue = minValue; }
		void setMaxValue(float maxValue) { this->maxValue = maxValue; }
		void increment(float value);
		void setPosition(sf::Vector2f pos);
		void setPosition(float x, float y) { this->setPosition(sf::Vector2f(x, y)); }
		void setLineColor(sf::Color color) { this->outerRect.setFillColor(color); }
		void setCircleColor(sf::Color color) { this->circle.setFillColor(color); }
		void setValue(float value);
		void setCircleRadius(float radius) { this->circle.setRadius(radius); }
		void setLength(int length) { this->outerRect.setSize(sf::Vector2f(length, this->outerRect.getSize().y)); }
		void setHeight(int height) { this->outerRect.setSize(sf::Vector2f(this->outerRect.getSize().x, height)); }
		void draw(sf::RenderWindow& window);
		void onClick(sf::Vector2f mouse) override;
		void onHover(sf::Vector2f mouse) override;
		void onDrag(sf::Vector2f mouse) override;
		void setOnDrag(std::function<void()>func) { this->dragFunctions = func; }
		void setOnClick(std::function<void()> func) { this->clickFunctions = func; }
		void setInnerRectColor(sf::Color color) { this->innerRectColor = color; this->innerRect.setFillColor(color); }
		void setOuterRectColor(sf::Color color) { this->outerRectColor = color; this->outerRect.setFillColor(color); }
		virtual ~Slider(){}
	private:
		void useDragFunction();
		void useClickFunction();
	};

	class ListItem
	{
	private:
		sf::Text text;
		sf::RectangleShape label;
		sf::Font font;
	public:
		ListItem(std::string text, float width, float height) : label(sf::Vector2f(width, height))
		{
			this->text.setString(text);
			this->text.setFillColor(sf::Color::Black);
			this->font.loadFromFile("sprites/arial.ttf");
			this->text.setFont(this->font);
		}
		void setColor(sf::Color color) { this->label.setFillColor(color); }
		void setText(std::string text) { this->text.setString(text); }
		void setPosition(float x, float y);
		void setCharacterSize(int size) { this->text.setCharacterSize(size); }
		void setPosition(sf::Vector2f pos);
		void draw(sf::RenderWindow& window);

		sf::RectangleShape getShape() { return this->label; }
	};

	class ListBox
	{
	private:
		std::vector<ListItem*> listItems;
		sf::RectangleShape listBox;
		sf::Color color;
		bool fitToContent = true;
	public:
		ListBox(float width, float height) : listBox(sf::Vector2f(width,height)){}
		void addItem(ListItem* listitem);
		void removeItem(ListItem listitem);
		void setColor(sf::Color color) { this->listBox.setFillColor(color); }
		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
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
		void updateNodesOnDrag(sf::Vector2f mouse) { for (Node* n : this->nodes) { n->onDrag(mouse); } }
		virtual ~Screen(){}
	};
}
#endif // !GUI_H