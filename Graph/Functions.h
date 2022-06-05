#pragma once
#include <SFML/Graphics.hpp>
#include "VertexCircle.h"
#include "Button.h"
#include "EdgeLine.h"
#include "cstdarg"

inline void DrawButtons(RenderWindow& Window, int count, ...)
{
	va_list button;
	va_start(button, count);
	for (int i = 0; i < count; i++)
	{
		Window.draw(va_arg(button, Button));
	}
}

inline void DrawVertexes(RenderWindow& Window, const std::vector<VertexCircle*>& drawing)
{
	for (int i = 0; i < drawing.size(); i++)
	{
		Window.draw(*drawing[i]);
	}
}

inline void DrawEdges(RenderWindow& Window, const std::vector<EdgeLine*>& drawing)
{
	for (int i = 0; i < drawing.size(); i++)
	{
		Window.draw(*drawing[i]);
	}
}

inline bool not_intersection(const std::vector<VertexCircle*>& v, const Vector2i& mouse)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i]->contain(mouse)) return false;
	}
	return true;
}

inline void VertexesDefaultColor(std::vector<VertexCircle*>& verts)
{
	for (int i = 0; i < verts.size(); i++)
	{
		verts[i]->set_color(VertexDefaultColor);
	}
}

inline void EdgesDefaultColor(std::vector<EdgeLine*>& edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		edges[i]->set_color(EdgeDefaultColor);
	}
}

inline void CheckVertecies(std::vector<VertexCircle*>& TmpVertexes)
{
	if (TmpVertexes.size() > 0)
	{
		VertexesDefaultColor(TmpVertexes);
		TmpVertexes.clear();
	}
}

inline void CheckEdges(std::vector<EdgeLine*>& TmpEdges)
{
	if (TmpEdges.size() > 0)
	{
		EdgesDefaultColor(TmpEdges);
		TmpEdges.clear();
	}
}

void SetCursor(RenderWindow& Window, const Vector2i& mousePos, std::vector<VertexCircle*>& VertexDrawingQueue, std::vector<EdgeLine*>& EdgeDrawingQueue, Button& AddVertexButton, Button& AddEdgeButton, Button& DeleteButton, Button& ApplyButton, Button& SalesmanButton, const bool& MustShowApply)
{
	bool isCursor = true;
	if (mousePos.y >= 0)
	{
		if (mousePos.x <= 0)//left
		{
			Cursor cursor;

			if (mousePos.y >= Window.getSize().y)//left down
			{
				if (cursor.loadFromSystem(Cursor::SizeBottomLeftTopRight))
					Window.setMouseCursor(cursor);
			}
			else
			{
				if (cursor.loadFromSystem(Cursor::SizeHorizontal))
					Window.setMouseCursor(cursor);
			}
			isCursor = false;
		}
		else if (mousePos.x >= Window.getSize().x)//right
		{
			Cursor cursor;

			if (mousePos.y >= Window.getSize().y)//right down
			{
				if (cursor.loadFromSystem(Cursor::SizeTopLeftBottomRight))
					Window.setMouseCursor(cursor);
			}
			else
			{
				if (cursor.loadFromSystem(Cursor::SizeHorizontal))
					Window.setMouseCursor(cursor);
			}
			isCursor = false;
		}
		else if (mousePos.y >= Window.getSize().y)//down
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::SizeVertical))
				Window.setMouseCursor(cursor);
			isCursor = false;
		}
	}
	else
	{
		isCursor = false;
	}

	for (int i = 0; i < VertexDrawingQueue.size() && isCursor; i++)
	{
		if (VertexDrawingQueue[i]->contain(mousePos))
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
			isCursor = false;
		}
	}

	if (isCursor)
	{
		if (AddVertexButton.contain(mousePos))
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
		}
		else if (AddEdgeButton.contain(mousePos))
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
		}
		else if (DeleteButton.contain(mousePos))
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
		}
		else if (ApplyButton.contain(mousePos) && MustShowApply)
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
		}
		else if (SalesmanButton.contain(mousePos))
		{
			Cursor cursor;
			if (cursor.loadFromSystem(Cursor::Hand))
				Window.setMouseCursor(cursor);
		}
		else
		{
			for (int i = 0; i < EdgeDrawingQueue.size() && isCursor; i++)
			{
				if (EdgeDrawingQueue[i]->contain(mousePos))
				{
					Cursor cursor;
					if (cursor.loadFromSystem(Cursor::Hand))
						Window.setMouseCursor(cursor);
					isCursor = false;
				}
			}
			if (isCursor)
			{
				Cursor cursor;
				if (cursor.loadFromSystem(Cursor::Arrow))
					Window.setMouseCursor(cursor);
			}
		}
	}
}

void VertexSettingsWindow(VertexCircle*& Vert, const std::vector<EdgeLine*>& EdgeDrawingQueue, Graph<int>& G)
{
	string old_name = Vert->getText();

	RenderWindow VertexSettings(VideoMode(400, 50), "VertexSettings", Style::Close | Style::Titlebar);

	Text message;
	Font font;
	if (!font.loadFromFile("timesnewromanpsmt.ttf")) exit(1);
	message.setFont(font);
	message.setFillColor(Color::Black);
	message.setCharacterSize(20);
	message.setPosition(2, 0);
	string new_name = old_name;
	message.setString("Input new vertex name:\n" + new_name);

	Text message1;
	message1.setFont(font);
	message1.setCharacterSize(20);
	message1.setPosition(2, 25);
	message1.setFillColor(Color(127, 127, 127));
	message1.setString("");
	while (VertexSettings.isOpen())
	{
		VertexSettings.clear(Color::White);

		VertexSettings.draw(message);
		if (message1.getString() != "")
		{
			VertexSettings.draw(message1);
		}
		VertexSettings.display();

		Event event;
		while (VertexSettings.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				VertexSettings.close();
				break;

			case Event::KeyPressed:
				if (message1.getString() == "")
				{
					if (event.key.code >= Keyboard::A && event.key.code <= Keyboard::Z)
					{
						new_name += (unsigned char)(event.key.code + 65);
						message.setString("Input new vertex name:\n" + new_name);
					}
					else if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9)
					{
						new_name += (unsigned char)(event.key.code + 22);
						message.setString("Input new vertex name:\n" + new_name);
					}
					else if (event.key.code >= Keyboard::Numpad0 && event.key.code <= Keyboard::Numpad9)
					{
						new_name += (unsigned char)(event.key.code - 27);
						message.setString("Input new vertex name:\n" + new_name);
					}
					else if (event.key.code == Keyboard::BackSpace)
					{
						if (new_name.size() != 0) new_name.erase(new_name.size() - 1);
						message.setString("Input new vertex name:\n" + new_name);
					}
					else if (event.key.code == Keyboard::Enter)
					{
						if (new_name != "" && new_name != old_name)
						{
							if (!G.is_vertex(new_name))
							{
								Vert->setText(new_name);
								for (int i = 0; i < EdgeDrawingQueue.size(); i++)
								{
									if (EdgeDrawingQueue[i]->get_start() == Vert)
									{
										EdgeDrawingQueue[i]->set_start(Vert);
									}
									else if (EdgeDrawingQueue[i]->get_end() == Vert)
									{
										EdgeDrawingQueue[i]->set_end(Vert);
									}
								}
								G.set_vertex_name(old_name, new_name);

								message.setString("Changes saved.");
								message.setFillColor(Color::Green);
							}
							else
							{
								message.setFillColor(Color::Red);
								message.setString("There is already a vertex with this name!");
							}
						}
						else
						{
							message.setFillColor(Color::Red);
							message.setString("The changes were not saved.");
						}

						message1.setString("Press any key to close this window.");
					}
					else if (event.key.code == Keyboard::Escape)
					{
						VertexSettings.close();
					}
				}

				else
				{
					VertexSettings.close();
				}
			}
		}
	}
}

void EdgeSettingsWindow(EdgeLine*& Edge, Graph<int>& G, std::vector<EdgeLine*> EdgeDrawingQueue)
{
	string old_weight = Edge->get_weight();
	RenderWindow EdgeSettings(VideoMode(400, 50), "EdgeSettings", Style::Close | Style::Titlebar);

	Text message;
	Font font;
	if (!font.loadFromFile("timesnewromanpsmt.ttf")) exit(1);
	message.setFont(font);
	message.setFillColor(Color::Black);
	message.setCharacterSize(20);
	message.setPosition(2, 0);
	string new_weight;
	if (old_weight == "0")
	{
		new_weight = "";
		old_weight = "";
	}
	else
	{
		new_weight = old_weight;
	}
	message.setString("Input new edge weight:\n" + new_weight);

	Text message1;
	message1.setFont(font);
	message1.setCharacterSize(20);
	message1.setPosition(2, 25);
	message1.setFillColor(Color(127, 127, 127));
	message1.setString("");

	while (EdgeSettings.isOpen())
	{
		EdgeSettings.clear(Color::White);

		EdgeSettings.draw(message);
		if (message1.getString() != "")
		{
			EdgeSettings.draw(message1);
		}
		EdgeSettings.display();

		Event event;
		while (EdgeSettings.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				EdgeSettings.close();
				break;

			case Event::KeyPressed:
				if (message1.getString() == "")
				{
					if (event.key.code >= Keyboard::Num0 && event.key.code <= Keyboard::Num9)
					{
						new_weight += (unsigned char)(event.key.code + 22);
						message.setString("Input new edge weight:\n" + new_weight);
					}
					else if (event.key.code >= Keyboard::Numpad0 && event.key.code <= Keyboard::Numpad9)
					{
						new_weight += (unsigned char)(event.key.code - 27);
						message.setString("Input new edge weight:\n" + new_weight);
					}
					else if (event.key.code == Keyboard::BackSpace)
					{
						if (new_weight.size() != 0) new_weight.erase(new_weight.size() - 1);
						message.setString("Input new edge weight:\n" + new_weight);
					}
					else if (event.key.code == Keyboard::Enter)
					{
						if (new_weight != "" && new_weight != old_weight)
						{
							if (new_weight == "0")//DELETE
							{
								std::vector<EdgeLine*>::iterator del;

								del = EdgeDrawingQueue.begin();
								for (del; del != EdgeDrawingQueue.end() && (*del != Edge); del++) {}
								G.delete_edge(Edge->get_start()->getText(), Edge->get_end()->getText());
								if (G.is_edge(Edge->get_end()->getText(), Edge->get_start()->getText()))
								{
									int j = 0;
									while (j < EdgeDrawingQueue.size() && (EdgeDrawingQueue[j]->get_start() != Edge->get_end() || EdgeDrawingQueue[j]->get_end() != Edge->get_start()))
									{
										j++;
									}
									EdgeDrawingQueue[j]->setAnotherEdge(false);
								}
								EdgeDrawingQueue.erase(del);
								delete Edge;
								Edge = nullptr;

								message.setString("Edge was deleted.");
								message.setFillColor(Color::Red);
							}
							else
							{
								Edge->set_weight(std::to_string(stoi(new_weight)));
								G.set_edge_weight(Edge->get_start()->getText(), Edge->get_end()->getText(), stoi(new_weight), true);
								message.setFillColor(Color::Green);
								message.setString("Changes were saved");
							}
						}
						else
						{
							message.setFillColor(Color::Red);
							message.setString("The changes were not saved.");
						}

						message1.setString("Press any key to close this window.");
					}
					else if (event.key.code == Keyboard::Escape)
					{
						EdgeSettings.close();
					}
				}

				else
				{
					EdgeSettings.close();
				}
			}
		}
	}
}

void MoveVertex(const RenderWindow& Window, VertexCircle*& movingVert, std::vector<EdgeLine*>& movingEdges, const Vector2i& mousePos, const Vector2i& prev_mousePos)
{
	const int limit = -2;
	const int indent = 5;

	if (movingVert->get_position().x - movingVert->get_radius() < -limit)//left
	{
		movingVert->set_color(VertexDefaultColor);
		movingVert->set_position(movingVert->get_radius() + indent, movingVert->get_position().y);

		if (movingVert->get_position().y - movingVert->get_radius() < limit)//left up
		{
			movingVert->set_position(movingVert->get_position().x, movingVert->get_radius() + indent);
		}
		else if (movingVert->get_position().y + movingVert->get_radius() > Window.getSize().y + limit)//left down
		{
			movingVert->set_position(movingVert->get_position().x, Window.getSize().y - movingVert->get_radius() - indent);
		}

		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->get_position().y - movingVert->get_radius() < -limit)//up
	{
		movingVert->set_color(VertexDefaultColor);
		movingVert->set_position(movingVert->get_position().x, movingVert->get_radius() + indent);

		if (movingVert->get_position().x + movingVert->get_radius() > Window.getSize().x + limit)//up right
		{
			movingVert->set_position(Window.getSize().x - movingVert->get_radius() - indent, movingVert->get_position().y);
		}

		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->get_position().x + movingVert->get_radius() > Window.getSize().x + limit)//right
	{
		movingVert->set_color(VertexDefaultColor);
		movingVert->set_position(Window.getSize().x - movingVert->get_radius() - indent, movingVert->get_position().y);

		if (movingVert->get_position().y + movingVert->get_radius() > Window.getSize().y + limit)//right down
		{
			movingVert->set_position(movingVert->get_position().x, Window.getSize().y - movingVert->get_radius() - indent);
		}

		movingVert = nullptr;
		movingEdges.clear();
	}
	else if (movingVert->get_position().y + movingVert->get_radius() > Window.getSize().y + limit)//down
	{
		movingVert->set_color(VertexDefaultColor);
		movingVert->set_position(movingVert->get_position().x, Window.getSize().y - movingVert->get_radius() - indent);

		movingVert = nullptr;
		movingEdges.clear();
	}
	else
	{
		movingVert->set_position(movingVert->get_position().x + (mousePos.x - prev_mousePos.x), (movingVert->get_position().y + (mousePos.y - prev_mousePos.y)));

		for (int i = 0; i < movingEdges.size(); i++)
		{
			if (movingVert == movingEdges[i]->get_start())	movingEdges[i]->set_start(movingVert);
			else if (movingVert == movingEdges[i]->get_end()) movingEdges[i]->set_end(movingVert);
		}
	}

	for (int i = 0; i < movingEdges.size(); i++)
	{
		if (movingVert == movingEdges[i]->get_start())	movingEdges[i]->set_start(movingVert);
		else if (movingVert == movingEdges[i]->get_end()) movingEdges[i]->set_end(movingVert);
	}
}

void SetResize(RenderWindow& Window, const Vector2i& prevWpos, const Vector2u& prevsize, Button& AddVertexButton, Button& AddEdgeButton, Button& DeleteButton, Button& ApplyButton, Button& SalesmanButton, TextMessage& BMessage, std::vector<VertexCircle*>& VertexDrawingQueue, std::vector<EdgeLine*>& EdgeDrawingQueue)
{
	if (Window.getSize().x < 800)
	{
		if (Window.getSize().y < 600)
		{
			Window.setSize(Vector2u(800, 600));
		}
		else
		{
			Window.setSize(Vector2u(800, Window.getSize().y));
		}
		Window.setPosition(prevWpos);
	}
	else if (Window.getSize().y < 600)
	{
		Window.setSize(Vector2u(Window.getSize().x, 600));
		Window.setPosition(prevWpos);
	}

	FloatRect visibleArea(0, 0, Window.getSize().x, Window.getSize().y);
	Window.setView(View(visibleArea));
	AddVertexButton.set_position(Window.getSize(), 50);
	AddEdgeButton.set_position(Window.getSize(), 100);
	DeleteButton.set_position(Window.getSize(), 150);
	ApplyButton.set_position(Window.getSize(), 300);
	SalesmanButton.set_position(Window.getSize(), 200);
	BMessage.set_string(Window, BMessage.get_string());

	for (int i = 0; i < VertexDrawingQueue.size(); i++)
	{
		VertexDrawingQueue[i]->set_position((float)VertexDrawingQueue[i]->get_position().x / ((float)prevsize.x / (float)Window.getSize().x), (float)VertexDrawingQueue[i]->get_position().y / ((float)prevsize.y / (float)Window.getSize().y));

		const int limit = -2;
		const int indent = 5;

		if (VertexDrawingQueue[i]->get_position().x - VertexDrawingQueue[i]->get_radius() < -limit)//left
		{
			VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_radius() + indent, VertexDrawingQueue[i]->get_position().y);

			if (VertexDrawingQueue[i]->get_position().y - VertexDrawingQueue[i]->get_radius() < limit)//left up
			{
				VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_position().x, VertexDrawingQueue[i]->get_radius() + indent);
			}
			else if (VertexDrawingQueue[i]->get_position().y + VertexDrawingQueue[i]->get_radius() > Window.getSize().y + limit)//left down
			{
				VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_position().x, Window.getSize().y - VertexDrawingQueue[i]->get_radius() - indent);
			}
		}
		else if (VertexDrawingQueue[i]->get_position().y - VertexDrawingQueue[i]->get_radius() < -limit)//up
		{
			VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_position().x, VertexDrawingQueue[i]->get_radius() + indent);

			if (VertexDrawingQueue[i]->get_position().x + VertexDrawingQueue[i]->get_radius() > Window.getSize().x + limit)//up right
			{
				VertexDrawingQueue[i]->set_position(Window.getSize().x - VertexDrawingQueue[i]->get_radius() - indent, VertexDrawingQueue[i]->get_position().y);
			}
		}
		else if (VertexDrawingQueue[i]->get_position().x + VertexDrawingQueue[i]->get_radius() > Window.getSize().x + limit)//right
		{
			VertexDrawingQueue[i]->set_position(Window.getSize().x - VertexDrawingQueue[i]->get_radius() - indent, VertexDrawingQueue[i]->get_position().y);

			if (VertexDrawingQueue[i]->get_position().y + VertexDrawingQueue[i]->get_radius() > Window.getSize().y + limit)//right down
			{
				VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_position().x, Window.getSize().y - VertexDrawingQueue[i]->get_radius() - indent);
			}
		}
		else if (VertexDrawingQueue[i]->get_position().y + VertexDrawingQueue[i]->get_radius() > Window.getSize().y + limit)//down
		{
			VertexDrawingQueue[i]->set_position(VertexDrawingQueue[i]->get_position().x, Window.getSize().y - VertexDrawingQueue[i]->get_radius() - indent);
		}

		for (int j = 0; j < EdgeDrawingQueue.size(); j++)
		{
			if (VertexDrawingQueue[i] == EdgeDrawingQueue[j]->get_start())	EdgeDrawingQueue[j]->set_start(VertexDrawingQueue[i]);
			else if (VertexDrawingQueue[i] == EdgeDrawingQueue[j]->get_end()) EdgeDrawingQueue[j]->set_end(VertexDrawingQueue[i]);
		}
	}
}