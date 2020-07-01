#include "gui.h"
#include <iostream>
#include <algorithm>

void gui::Button::setPosition(float width,float height)
{
	this->setPosition(sf::Vector2f(width, height));
}

void gui::Button::setPosition(sf::Vector2f position)
{
	//set button position
	this->buttonShape.setPosition(position);
	//set the text inside the button to maintain its position
	std::cout << this->textWidth << std::endl;
	sf::FloatRect textRect = text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
	sf::Vector2f positionText((position.x + (this->buttonShape.getSize().x/2)) , position.y + this->buttonShape.getSize().y / 2);
	std::cout << positionText.x << "  " << positionText.y << std::endl;
	this->text.setPosition(positionText);
}

void gui::Button::draw(sf::RenderWindow& window)
{
	window.draw(this->buttonShape);
	window.draw(this->text);
}

void gui::Button::onClick(sf::Vector2f mouse)
{
	auto bounds = this->buttonShape.getLocalBounds();
	auto pos = this->buttonShape.getPosition();
	if (bounds.contains(sf::Vector2f((mouse.x - pos.x),(mouse.y-pos.y))))
	{
		std::cout << "a button was clicked with the following coordinates: " << mouse.x << " " << mouse.y << std::endl;
	}
}

void gui::Button::onHover(sf::Vector2f mouse)
{
	auto bounds = this->buttonShape.getLocalBounds();
	auto pos = this->buttonShape.getPosition();
	if (bounds.contains(sf::Vector2f((mouse.x - pos.x), (mouse.y - pos.y))))
	{
		this->buttonShape.setFillColor(this->hoverColor);
	}
	else
	{
		this->buttonShape.setFillColor(this->color);
	}
}
