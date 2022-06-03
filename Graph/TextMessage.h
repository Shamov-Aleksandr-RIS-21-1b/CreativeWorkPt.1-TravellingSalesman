#pragma once
class TextMessage : public Drawable
{
private:
	sf::Text message;
	sf::Font font;
public:
	TextMessage(const sf::RenderWindow& Window, const string& message)
	{
		if (!font.loadFromFile("timesnewromanpsmt.ttf")) exit(1);
		this->message.setFont(font);
		this->message.setFillColor(sf::Color(127, 127, 127));
		this->message.setCharacterSize(20);
		this->message.setString(message);
		sf::FloatRect rect = this->message.getGlobalBounds();
		this->message.setOrigin(rect.width / 2, rect.height / 2);
		this->message.setPosition(Window.getSize().x / 2, Window.getSize().y - 50);
	}
	TextMessage() {};
	void set_string(const RenderWindow& Window, const string& message)
	{
		this->message.setString(message);
		sf::FloatRect rect = this->message.getGlobalBounds();
		this->message.setOrigin(rect.width / 2, rect.height / 2);
		this->message.setPosition(Window.getSize().x / 2, Window.getSize().y - 50);
	}
	string get_string()
	{
		return message.getString();
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override
	{
		if (message.getString() != "")
			target.draw(message);
	}
};