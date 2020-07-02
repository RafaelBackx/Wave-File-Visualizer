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
	sf::FloatRect textRect = text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
	sf::Vector2f positionText((position.x + (this->buttonShape.getSize().x/2)) , position.y + this->buttonShape.getSize().y / 2);
	this->text.setPosition(positionText);
}

void gui::Button::draw(sf::RenderWindow& window)
{
	window.draw(this->buttonShape);
	window.draw(this->text);
}

void gui::Button::onClick(sf::Vector2f mouse)
{
	auto bounds = this->buttonShape.getGlobalBounds();
	if (bounds.contains(sf::Vector2f(mouse.x,(mouse.y))))
	{
		useClickFunction();
	}
}

void gui::Button::onHover(sf::Vector2f mouse)
{
	auto bounds = this->buttonShape.getGlobalBounds();
	if (bounds.contains(sf::Vector2f(mouse.x , mouse.y)))
	{
		this->buttonShape.setFillColor(this->hoverColor);
	}
	else
	{
		this->buttonShape.setFillColor(this->color);
	}
}

void gui::Button::useClickFunction()
{
	switch (this->functions.index())
	{
	case 0:
	{
		auto func = std::get<std::function<void()>>(this->functions);
		func();
		break;
	}
	case 1:
	{
		auto func = std::get <std::function<void(Button & btn)>>(this->functions);
		func(*this);
		break;
	}
	default:
		break;
	}
}

void gui::Slider::onClick(sf::Vector2f mouse)
{
	auto boundsCircle = this->circle.getGlobalBounds();
	auto boundsLine = this->line.getGlobalBounds();
	if (boundsCircle.contains(mouse) || boundsLine.contains(mouse))
	{
		this->circle.move(10.0f, 0.0f);
		//std::cout << "clicked" << mouse.x << " " << mouse.y << std::endl;
	}
}

void gui::Slider::onHover(sf::Vector2f mouse)
{
	//auto boundsCircle = this->circle.getGlobalBounds();
	//auto boundsLine = this->line.getGlobalBounds();
	//if (boundsCircle.contains(mouse) || boundsLine.contains(mouse))
	//{
	//	//std::cout << "Hovered" << mouse.x << " " << mouse.y << std::endl;
	//}
}

void gui::Slider::setPosition(sf::Vector2f pos)
{
	this->line.setPosition(pos);
	sf::Vector2f circlePos = pos;
	circlePos.y += this->line.getSize().y / 2;
	this->circle.setPosition(circlePos);
}

void gui::Slider::draw(sf::RenderWindow& window)
{
	window.draw(this->line);
	window.draw(this->circle);
}
