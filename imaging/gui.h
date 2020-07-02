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
		void setTexture(sf::Texture texture) { this->texture = texture; this->buttonShape.setTexture(&this->texture); }
		void setTexture(std::string pathToTexture)
		{
			if (!this->texture.loadFromFile(pathToTexture))
			{
				std::cout << "Could not load " << pathToTexture << std::endl;
			}
			else
			{
				this->buttonShape.setTexture(&this->texture);
			}
		}
		void onClick(sf::Vector2f mouse) override;
		void onHover(sf::Vector2f mouse) override;
		void draw(sf::RenderWindow& window);
		virtual ~Button(){}
	private:
		void useClickFunction();
	};

	class Slider : public Node
	{
	private:
		sf::RectangleShape line;
		sf::CircleShape circle;
		float value;
	public:
		/*Slider(int lineWidth, int lineHeight, float circleRadius) : Slider(lineWidth,lineHeight,circleRadius,0){}*/

		Slider(int lineWidth, int lineHeight,float circleRadius,float value): line(sf::Vector2f(lineWidth,lineHeight)), value(value)
		{
			this->circle.setRadius(circleRadius);
			auto bounds = this->circle.getLocalBounds();
			this->circle.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		}

		const float getCircleRadius()const { return this->circle.getRadius(); }
		const int getLineLength()const { return this->line.getSize().x; }
		const int getLineHeight()const { return this->line.getSize().y; }
		const float getValue() const { return this->value; }

		
		void setPosition(sf::Vector2f pos);
		void setPosition(float x, float y) { this->setPosition(sf::Vector2f(x, y)); }
		void setLineColor(sf::Color color) { this->line.setFillColor(color); }
		void setCircleColor(sf::Color color) { this->circle.setFillColor(color); }
		void setValue(float value) { this->value = value; }
		void setCircleRadius(float radius) { this->circle.setRadius(radius); }
		void setLength(int length) { this->line.setSize(sf::Vector2f(length, this->line.getSize().y)); }
		void setHeight(int height) { this->line.setSize(sf::Vector2f(this->line.getSize().x, height)); }
		void draw(sf::RenderWindow& window);
		void onClick(sf::Vector2f mouse) override;
		void onHover(sf::Vector2f mouse) override;
		virtual ~Slider(){}
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