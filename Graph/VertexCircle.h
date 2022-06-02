#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
using namespace sf;
using std::string;
const double PI = 3.14;

const Color VertexDefaultColor = Color(193, 100, 113);
const Color VertexHighlightedColor = Color(240, 73, 112);

class VertexCircle : public Drawable
{
public:
	CircleShape C;
	Text text;
	Font font;
public:
	VertexCircle(const string& name)
	{
		bool f = font.loadFromFile("timesnewromanpsmt.ttf");
		if (!f) exit(1);
		C.setRadius(50);
		C.setFillColor(VertexDefaultColor);
		C.setOutlineColor(Color::Black);
		C.setOutlineThickness(3);
		text.setFont(font);
		C.setOrigin(C.getRadius(), C.getRadius());
		C.setPosition(100, 100);
		text.setCharacterSize(C.getRadius() / 3);
		text.setString(name);
		text.setFillColor(Color::Black);
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(C.getPosition().x, C.getPosition().y - text.getCharacterSize() / 4);
	}
	void set_radius(const int& radius)
	{
		C.setRadius(radius);
	}
	void set_color(const Color& color)
	{
		C.setFillColor(color);
	}
	void set_outline(const Color& color)
	{
		C.setOutlineColor(color);
	}
	void set_thick(const int& thick)
	{
		C.setOutlineThickness(thick);
	}
	void set_position(const float& x, const float& y)
	{
		C.setPosition(x, y);
		text.setPosition(C.getPosition().x, C.getPosition().y - text.getCharacterSize() / 4);
	}
	void set_font(const string& filename)
	{
		Font font;
		bool f = font.loadFromFile(filename);
		text.setFont(font);
		if (!f) exit(1);
	}
	void set_text_color(const Color& color)
	{
		text.setFillColor(color);
	}
	bool contain(const Vector2i& mouse)
	{
		return C.getGlobalBounds().contains(mouse.x, mouse.y);
	}
	string getText()
	{
		return text.getString();
	}
	void setText(const string& s)
	{
		text.setString(s);
		FloatRect textRect = text.getLocalBounds();
		if (textRect.width / 2 > C.getRadius())
		{
			C.setRadius(textRect.width / 2);
		}

		C.setOrigin(C.getRadius(), C.getRadius());
		text.setOrigin(textRect.width / 2, textRect.height / 2);
		text.setPosition(C.getPosition().x, C.getPosition().y - text.getCharacterSize() / 4);
	}
	Vector2f get_position()
	{
		return C.getPosition();
	}
	Color get_color()
	{
		return C.getFillColor();
	}
	int get_radius()
	{
		return C.getRadius();
	}
	void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override
	{
		target.draw(C);
		target.draw(text);
	}
};