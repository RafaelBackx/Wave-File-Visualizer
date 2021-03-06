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

void gui::Button::onDrag(sf::Vector2f mouse)
{
	
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
	auto boundsOuterRect = this->outerRect.getGlobalBounds();
	if (boundsCircle.contains(mouse) || boundsOuterRect.contains(mouse))
	{
		if (mouse.x >= boundsOuterRect.left && mouse.x < boundsOuterRect.left + boundsOuterRect.width)
		{
			this->circle.setPosition(sf::Vector2f(mouse.x, this->circle.getPosition().y));
			float cx = this->circle.getPosition().x;
			float value = cx - boundsOuterRect.left;
			this->value = ((value / (boundsOuterRect.width)) * maxValue + minValue);
			sf::Vector2f innerRectSize(cx - boundsOuterRect.left,this->innerRect.getSize().y);
			this->innerRect.setSize(innerRectSize);
			this->useClickFunction();
		}
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

void gui::Slider::onDrag(sf::Vector2f mouse)
{
	auto boundsCircle = this->circle.getGlobalBounds();
	auto boundsOuterRect = this->outerRect.getGlobalBounds();
	if (boundsCircle.contains(mouse) || boundsOuterRect.contains(mouse))
	{
		if (mouse.x >= boundsOuterRect.left && mouse.x < boundsOuterRect.left+boundsOuterRect.width)
		{
			this->circle.setPosition(sf::Vector2f(mouse.x, this->circle.getPosition().y));
			float cx = this->circle.getPosition().x;
			float value = cx - boundsOuterRect.left;
			this->value = ((value/(boundsOuterRect.width))*maxValue + minValue);
			sf::Vector2f innerRectSize(cx - boundsOuterRect.left, this->innerRect.getSize().y);
			this->innerRect.setSize(innerRectSize);
			this->useDragFunction();
		}
		//std::cout << "clicked" << mouse.x << " " << mouse.y << std::endl;
	}
}

void gui::Slider::setPosition(sf::Vector2f pos)
{
	this->outerRect.setPosition(pos);
	this->innerRect.setPosition(pos);
	sf::Vector2f circlePos = pos;
	circlePos.y += this->outerRect.getSize().y / 2;
	this->circle.setPosition(circlePos);
}

void gui::Slider::draw(sf::RenderWindow& window)
{
	window.draw(this->outerRect);
	window.draw(this->innerRect);
	window.draw(this->circle);
}

void gui::Slider::useDragFunction()
{
	switch (this->dragFunctions.index())
	{
	case 0:
	{
		auto func = std::get<std::function<void()>>(this->dragFunctions);
		func();
		break;
	}
	default:
		break;
	}
}

void gui::Slider::useClickFunction()
{
	switch (this->clickFunctions.index())
	{
	case 0:
	{
		auto func = std::get<std::function<void()>>(this->clickFunctions);
		func();
		break;
	}
	default:
		break;
	}
}

void gui::Slider::setValue(float value)
{
	if (value >= minValue && value <= maxValue)
	{
		this->value = value;
		sf::FloatRect boundsOuterRect = this->outerRect.getGlobalBounds();
		float circleX = boundsOuterRect.left + (boundsOuterRect.width * ((value-minValue) / maxValue));
		sf::Vector2f pos(circleX, this->circle.getPosition().y);
		this->circle.setPosition(pos);
		sf::Vector2f sizeInnerRect(circleX - boundsOuterRect.left, this->innerRect.getSize().y);
		this->innerRect.setSize(sizeInnerRect);
		//useClickFunction();
	}
}

void gui::Slider::increment(float value)
{
	this->setValue(this->getValue() + value);
}

void gui::ListItem::setPosition(float x, float y)
{
	sf::Vector2f pos(x, y);
	this->setPosition(pos);
}

void gui::ListItem::setPosition(sf::Vector2f pos)
{
	this->label.setPosition(pos);
	auto textBounds = this->text.getLocalBounds();
	this->text.setOrigin(textBounds.left + (textBounds.width / 2.0f), textBounds.top + (textBounds.height / 2.0f));
	this->text.setPosition(pos.x + (this->label.getSize().x / 2.0f), pos.y + (this->label.getSize().y / 2.0f));
}

void gui::ListItem::draw(sf::RenderWindow& window)
{
	window.draw(this->label);
	window.draw(this->text);
}

void gui::ListItem::setText(std::string text)
{
	this->text.setString(text);
	auto bounds = this->text.getLocalBounds();
	this->label.setSize(sf::Vector2f(bounds.width+padding, bounds.height+padding));
}

void gui::ListItem::setWidth(float width)
{
	auto size = this->label.getSize();
	this->label.setSize(sf::Vector2f(width, size.y));
}

void gui::ListItem::setPadding(float padding)
{
	this->padding = padding;
	sf::Vector2f size = this->label.getSize();
	this->label.setSize(sf::Vector2f(size.x + padding, size.y+padding));
}

void gui::ListBox::setPosition(float x, float y)
{
	this->setPosition(sf::Vector2f(x, y));
}

void gui::ListBox::setPosition(sf::Vector2f pos)
{
	this->listBox.setPosition(pos);
	float x = pos.x, y = pos.y;
	for (int i = 0; i < this->listItems.size(); i++)
	{
		this->listItems[i]->setPosition(x, y);
		y += this->listItems[i]->getShape().getSize().y;
	}
}

void gui::ListBox::addItem(std::unique_ptr<ListItem> item)
{
	this->listItems.push_back(std::move(item));
	if (fitToContent)
	{
		float x = 0, y = 0;
		for (int i=0; i<this->listItems.size();i++)
		{
			if (this->listItems[i]->getSize().x> x)
			{
				x = this->listItems[i]->getSize().x;
			}
			y += this->listItems[i]->getShape().getSize().y;
		}
		sf::Vector2f size(x,y);
		this->listBox.setSize(size);
	}
	
}

void gui::ListBox::setFitToContent(bool fit)
{
	this->fitToContent = fit;
}

void gui::ListBox::draw(sf::RenderWindow& window)
{
	window.draw(this->listBox);
	for (int i=0;i<this->listItems.size();i++)
	{
		this->listItems[i]->draw(window);
	}
}

void gui::TextBox::addText(std::string string)
{
	if (focus)
	{
		std::string totalText = this->getText();
		std::string beforeOffset = totalText.substr(0, textOffset);
		std::string afterOffset = totalText.substr(textOffset, totalText.length() - textOffset);
		std::string newText = beforeOffset + string + afterOffset;
		this->text.setString(newText);
		auto bounds = this->text.getLocalBounds();
		sf::Vector2f newSize(bounds.width + padding * 2, bounds.height + padding * 2);
		this->box.setSize(this->checkSize(newSize));
		this->setTextOffset(textOffset + 1);
	}
}

void gui::TextBox::clear()
{
	this->text.setString("");
	auto bounds = this->text.getLocalBounds();
	this->box.setSize(sf::Vector2f(bounds.width,bounds.height));
}

void gui::TextBox::setPosition(float x, float y)
{
	this->setPosition(sf::Vector2f(x, y));
}

void gui::TextBox::setPosition(sf::Vector2f pos)
{
	this->box.setPosition(pos);
	this->text.setPosition(pos);
}

void gui::TextBox::draw(sf::RenderWindow& window)
{
	window.draw(this->box);
	window.draw(this->text);
	float mSeconds = this->clock.getElapsedTime().asSeconds();
	if (this->pointer.size()>0 && mSeconds<1) // draw the line for 1 second
	{
		window.draw(&this->pointer[0], 2, sf::Lines);
	}
	else if (mSeconds >1.5) // when it passes 1,5 seconds restart the clock
	{
		clock.restart();
	}
}

void gui::TextBox::setString(std::string text)
{
	this->text.setString(text);
	auto bounds = this->text.getLocalBounds();
	sf::Vector2f newSize(bounds.width + padding * 2, bounds.height + padding * 2);
	this->box.setSize(this->checkSize(newSize));
}

void gui::TextBox::setPadding(float padding)
{
	auto pos = this->text.getPosition();
	this->text.setPosition(pos.x + padding, pos.y + padding);
	this->padding = padding;
	auto bounds = this->text.getLocalBounds();
	sf::Vector2f newSize(bounds.width + (padding * 2), bounds.height + (padding * 2));
	this->box.setSize(this->checkSize(newSize));
}

void gui::TextBox::removeLastChar()
{
	if (focus)
	{
		std::string text = this->getText();
		if (textOffset >0)
		{
			std::string beforeOffset = text.substr(0, textOffset - 1);
			std::string afterOffset = text.substr(textOffset, text.length() - textOffset);
			this->setString(beforeOffset + afterOffset);
			this->decrementTextOffset();
		}
	}
}

void gui::TextBox::onClick(sf::Vector2f mouse)
{
	auto bounds = this->box.getGlobalBounds();
	if (bounds.contains(sf::Vector2f(mouse.x, (mouse.y))))
	{
		this->focus = true;
		useClickFunction();
	}
}

void gui::TextBox::useClickFunction()
{
	switch (this->clickFunctions.index())
	{
	case 0:
	{
		auto func = std::get<std::function<void()>>(this->clickFunctions);
		func();
		break;
	}
	default:
		break;
	}
}

sf::Vector2f gui::TextBox::checkSize(sf::Vector2f newSize)
{
	if (newSize.x < this->defaultSize.x && newSize.y < this->defaultSize.y)
	{
		return defaultSize;
	}
	else
	{
		return newSize;
	}
}

void gui::TextBox::setTextOffset(int offset)
{
	int length = this->getText().length();
	if (offset >= 0 && offset <= length)
	{
		std::cout << "offset: " << offset << " length: " << length << std::endl;
		this->textOffset = offset;
		sf::Vector2f pos;
		int width = 15;
		if (this->getText().length() > 0)
		{
			pos = this->text.findCharacterPos(offset);
			//width = this->text.getLocalBounds().width / this->getText().length();
		}
		else
		{
			pos = sf::Vector2f(this->box.getGlobalBounds().left, this->box.getGlobalBounds().top);
		}
		this->pointer.clear();
		this->pointer.push_back(sf::Vertex(sf::Vector2(pos.x, pos.y + this->text.getCharacterSize()), sf::Color::Black));
		this->pointer.push_back(sf::Vertex(sf::Vector2(pos.x + width, pos.y + this->text.getCharacterSize()), sf::Color::Black));
		std::cout << pos.x << " " << pos.y << std::endl;
		std::cout << pos.x + width << " " << pos.y << std::endl;
	}
}

void gui::TextBox::handleEvent(sf::Event event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128)
		{
			if (event.text.unicode == 8) // check for backspace
			{
				this->removeLastChar();
			}
			else if (event.text.unicode == 22 && event.key.control) // check for control paste
			{
				this->setString(clipboard.getString());
			}
			else if (event.text.unicode == 3 && event.key.control) // check for control copy
			{
				clipboard.setString(this->getText());
			}
			else
			{
				char t = static_cast<char>(event.text.unicode);
				this->addText(std::string(1, t));
				int index = this->getText().length() - 1;
				if (this->getSFMLText().findCharacterPos(index).x > window.getSize().x - 50)
				{
					this->addText(std::string("\n"));
				}
			}
		}
	}
}

void gui::Screen::removeNode(gui::Node* node)
{
	int iterator = -1;
	for (int i =0;i<this->nodes.size();++i)
	{
		if (nodes[i] == node)
		{
			iterator = i;
		}
	}
	if (iterator>0)
	{
		this->nodes.erase(this->nodes.begin() + iterator);
	}
}