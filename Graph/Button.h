#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using std::string;

enum ButtonModes
{
	Off = 0,
	On = 1,
};

const Color OffButtonColor = Color(77, 101, 104);
const Color OnButtonColor = Color(116, 162, 158);

class Button : public Drawable
{

private:
	RectangleShape B;
	Text text;
	Font font;
	int mode;
public:
	Button(const string& info, const Vector2u& window, const int& y_indent)
	{
		bool f = font.loadFromFile("timesnewromanpsmt.ttf");
		if (!f) exit(1);
		text.setCharacterSize(20);
		text.setFont(font);
		text.setString(info);
		text.setFillColor(Color::White);
		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.width / 2, textRect.height / 2);

		B.setFillColor(OffButtonColor);
		B.setOutlineColor(Color::Black);
		B.setOutlineThickness(1);
		B.setSize(Vector2f(150, 30));
		FloatRect buttonRect = B.getLocalBounds();
		B.setOrigin(buttonRect.width, 0);
		B.setPosition(window.x - 20, y_indent);

		text.setPosition(B.getPosition().x - buttonRect.width / 2, B.getPosition().y + buttonRect.height / 4);

		mode = Off;
	}
	void set_position(const Vector2u& window, const int& y_indent)
	{
		FloatRect buttonRect = B.getLocalBounds();
		B.setOrigin(buttonRect.width, 0);
		B.setPosition(window.x - 20, y_indent);

		text.setPosition(B.getPosition().x - buttonRect.width / 2, B.getPosition().y + buttonRect.height / 4);
	}
	bool contain(const Vector2i& mouse)
	{
		return B.getGlobalBounds().contains(mouse.x, mouse.y);
	}
	void set_color(Color color)
	{
		B.setFillColor(color);
	}
	void setMode(const int& Mode)
	{
		this->mode = Mode;
		if (Mode == On) B.setFillColor(OnButtonColor);
		else if (Mode == Off) B.setFillColor(OffButtonColor);
	}
	int Mode()
	{
		return mode;
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override
	{
		target.draw(B);
		target.draw(text);
	}
};