#include<iostream>
#include "Graph.h"
#include <SFML/Graphics.hpp>
#include "VertexCircle.h"
#include "Button.h"
#include "TextMessage.h"
#include "Functions.h"
#include "EdgeLine.h"
using namespace sf;
using namespace std;

const Color WindowColor = Color(240, 211, 193);
const int SpawnX = 60;
const int SpawnY = 60;
const int AllButtonsTextSize = 50;

int main()
{
	Graph<int> G;

	RenderWindow Window(VideoMode(800, 600), "GraphConstructor");
	Window.setVerticalSyncEnabled(true);
	int vi = 1;

	Button AddVertexButton("add vertex", Window.getSize(), 50);
	Button AddEdgeButton("add edge", Window.getSize(), 100);
	Button DeleteButton("delete", Window.getSize(), 150);

	Button SalesmanButton("Salesman", Window.getSize(), 200);

	Button ApplyButton("apply", Window.getSize(), 300);

	vector<VertexCircle*> VertexDrawingQueue;
	vector<EdgeLine*> EdgeDrawingQueue;
	VertexCircle* movingVert = nullptr;
	vector<EdgeLine*> movingEdges;
	VertexCircle* AddedVertex = nullptr;
	EdgeLine* AddedEdge = nullptr;

	TextMessage BMessage(Window, "");

	vector<VertexCircle*> TmpVertexes;
	vector<EdgeLine*> TmpEdges;

	bool AnyButtonWasOn = false;
	bool MustShowApply = false;

	Vector2i prev_mousePos;

	while (Window.isOpen())
	{
		Vector2i mousePos = Mouse::getPosition(Window);
		Vector2u prevsize = Window.getSize();
		Vector2i prevWpos = Window.getPosition();

		Window.clear(WindowColor);
		DrawEdges(Window, EdgeDrawingQueue);
		DrawButtons(Window, 4, AddVertexButton, AddEdgeButton, DeleteButton, SalesmanButton);
		if (MustShowApply) Window.draw(ApplyButton);
		DrawVertexes(Window, VertexDrawingQueue);
		Window.draw(BMessage);
		Window.display();
		
		SetCursor(Window, mousePos, VertexDrawingQueue, EdgeDrawingQueue, AddVertexButton, AddEdgeButton, DeleteButton, ApplyButton, SalesmanButton);

		Event event;
		while (Window.pollEvent(event))
		{
			switch (event.type)
			{

			case Event::Closed:
				Window.close();
				break;

			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left)
				{
					if (AddVertexButton.contain(mousePos) && not_intersection(VertexDrawingQueue, mousePos))
					{
						if (AnyButtonWasOn)
						{
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							SalesmanButton.setMode(Off);
							DeleteButton.setMode(Off);
							AddEdgeButton.setMode(Off);
						}
						AddVertexButton.setMode(On);
						BMessage.set_string(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
						AddedVertex = new VertexCircle("V" + to_string(vi));
						AddedVertex->set_position(SpawnX, SpawnY);
						AddedVertex->set_color(VertexHighlightedColor);
					    VertexDrawingQueue.push_back(AddedVertex);
					    G.add_vertex("V" + to_string(vi));
					    vi++;
					}

					if (AddEdgeButton.contain(mousePos) && not_intersection(VertexDrawingQueue, mousePos))
					{
						if (AddEdgeButton.Mode() == On)
						{
							if (VertexDrawingQueue.size() > 0) BMessage.set_string(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.set_string(Window, "");
							AddEdgeButton.setMode(Off);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (AddEdgeButton.Mode() == Off)
						{
							if (AnyButtonWasOn)
							{
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								DeleteButton.setMode(Off);
								SalesmanButton.setMode(Off);
							}
							BMessage.set_string(Window, "Click on two vertices sequentially to build an edge between them.");
							AddEdgeButton.setMode(On);
							AnyButtonWasOn = true;
						}
					}
					else if (AddEdgeButton.Mode() == On)
					{
						bool Continue = true;
						for (int i = VertexDrawingQueue.size() - 1; i >= 0 && Continue; i--)
						{
							if (VertexDrawingQueue[i]->contain(mousePos))
							{
								if (VertexDrawingQueue[i]->get_color() == VertexDefaultColor)
								{
									if (TmpVertexes.size() < 2)
									{
										VertexDrawingQueue[i]->set_color(VertexHighlightedColor);
										TmpVertexes.push_back(VertexDrawingQueue[i]);
									}
								}
								else
								{
									VertexDrawingQueue[i]->set_color(VertexDefaultColor);

									vector<VertexCircle*>::iterator it = TmpVertexes.begin();
									for (it; it != TmpVertexes.end() && *it != VertexDrawingQueue[i]; it++) {}
									TmpVertexes.erase(it);
								}
								Continue = false;
							}
						}
						MustShowApply = TmpVertexes.size() == 2;
					}

					if (DeleteButton.contain(mousePos) && not_intersection(VertexDrawingQueue, mousePos))
					{
						if (DeleteButton.Mode() == On)
						{
							if (VertexDrawingQueue.size() > 0) BMessage.set_string(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.set_string(Window, "");
							DeleteButton.setMode(Off);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (DeleteButton.Mode() == Off)
						{
							if (AnyButtonWasOn)
							{
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								AddEdgeButton.setMode(Off);
								SalesmanButton.setMode(Off);
							}
							BMessage.set_string(Window, "Click the LMB on the vertex or edge to delete it.");
							DeleteButton.setMode(On);
							AnyButtonWasOn = true;
						}
					}
					else if (DeleteButton.Mode() == On)
					{
						bool Continue = true;
						for (int i = VertexDrawingQueue.size() - 1; i >= 0 && Continue; i--)
						{
							if (VertexDrawingQueue[i]->contain(mousePos))
							{
								if (VertexDrawingQueue[i]->get_color() == VertexDefaultColor)
								{
									VertexDrawingQueue[i]->set_color(VertexHighlightedColor);
									TmpVertexes.push_back(VertexDrawingQueue[i]);
									for (int j = 0; j < EdgeDrawingQueue.size(); j++)
									{
										if (EdgeDrawingQueue[j]->containVert(VertexDrawingQueue[i]) && EdgeDrawingQueue[j]->get_color() == EdgeDefaultColor)
										{
											EdgeDrawingQueue[j]->set_color(EdgeHighlightedColor);
											TmpEdges.push_back(EdgeDrawingQueue[j]);
										}
									}
								}
								else
								{
									VertexDrawingQueue[i]->set_color(VertexDefaultColor);
									vector<VertexCircle*>::iterator it = TmpVertexes.begin();
									for (it; it != TmpVertexes.end() && *it != VertexDrawingQueue[i]; it++) {}
									TmpVertexes.erase(it);
									for (int j = 0; j < TmpEdges.size(); j++)
									{
										if (TmpEdges[j]->get_start() == VertexDrawingQueue[i])
										{
											if (TmpEdges[j]->get_end()->get_color() == VertexDefaultColor)
											{
												TmpEdges[j]->set_color(EdgeDefaultColor);
												vector<EdgeLine*>::iterator it = TmpEdges.begin() + j;
												TmpEdges.erase(it);
												--j;
											}
										}
										else if (TmpEdges[j]->get_end() == VertexDrawingQueue[i])
										{
											if (TmpEdges[j]->get_start()->get_color() == VertexDefaultColor)
											{
												TmpEdges[j]->set_color(EdgeDefaultColor);
												vector<EdgeLine*>::iterator it = TmpEdges.begin() + j;
												TmpEdges.erase(it);
												--j;
											}
										}
									}
								}
								Continue = false;
							}
						}

						for (int i = EdgeDrawingQueue.size() - 1; i >= 0 && Continue; i--)
						{
							if (EdgeDrawingQueue[i]->contain(mousePos))
							{
								if (EdgeDrawingQueue[i]->get_color() == EdgeDefaultColor)
								{
									for (int j = 0; j < TmpVertexes.size(); j++)
									{
										if (TmpVertexes[j] == EdgeDrawingQueue[i]->get_start() || TmpVertexes[j] == EdgeDrawingQueue[i]->get_end())
										{
											Continue = false;
										}
									}
									if (Continue)
									{
										EdgeDrawingQueue[i]->set_color(EdgeHighlightedColor);
										TmpEdges.push_back(EdgeDrawingQueue[i]);
									}
								}
								else
								{
									for (int j = 0; j < TmpVertexes.size(); j++)
									{
										if (!EdgeDrawingQueue[i]->containVert(TmpVertexes[j]))
										{
											EdgeDrawingQueue[i]->set_color(EdgeDefaultColor);
											vector<EdgeLine*>::iterator it = TmpEdges.begin();
											for (it; it != TmpEdges.end() && *it != EdgeDrawingQueue[i]; it++) {}
											TmpEdges.erase(it);
										}
									}
								}
								Continue = false;
							}
						}

						MustShowApply = TmpVertexes.size() > 0 || TmpEdges.size() > 0;
					}

					if (ApplyButton.contain(mousePos) && not_intersection(VertexDrawingQueue, mousePos))
					{
						ApplyButton.setMode(On);

						if (DeleteButton.Mode() == On)
						{
							for (int i = 0; i < TmpEdges.size(); i++)
							{
								vector<EdgeLine*>::iterator del;

								del = EdgeDrawingQueue.begin();
								for (del; del != EdgeDrawingQueue.end() && (*del != TmpEdges[i]); del++) {}
								G.delete_edge(TmpEdges[i]->get_start()->getText(), TmpEdges[i]->get_end()->getText());
								if (G.is_edge(TmpEdges[i]->get_end()->getText(), TmpEdges[i]->get_start()->getText()))
								{
									int j = 0;
									while (j < EdgeDrawingQueue.size() && (EdgeDrawingQueue[j]->get_start() != TmpEdges[i]->get_end() || EdgeDrawingQueue[j]->get_end() != TmpEdges[i]->get_start()))
									{
										j++;
									}
									EdgeDrawingQueue[j]->setAnotherEdge(false);
								}
								EdgeDrawingQueue.erase(del);
								delete TmpEdges[i];
								TmpEdges[i] = nullptr;
							}
							TmpEdges.clear();

							for (int i = 0; i < TmpVertexes.size(); i++)
							{
								vector<VertexCircle*>::iterator del;

								del = VertexDrawingQueue.begin();
								for (del; del != VertexDrawingQueue.end() && (*del != TmpVertexes[i]); del++) {}
								VertexDrawingQueue.erase(del);

								G.delete_vertex(TmpVertexes[i]->getText());

								delete TmpVertexes[i];
								TmpVertexes[i] = nullptr;
							}
							TmpVertexes.clear();
						}
						else if (AddEdgeButton.Mode() == On)
						{
							switch (VertexDrawingQueue.size())
							{
							case 2:
								if (EdgeDrawingQueue.size() <= 1)
								{
									if (!G.is_edge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText()))
									{
										bool AnotherEdge = G.is_edge(TmpVertexes[1]->getText(), TmpVertexes[0]->getText());
										AddedEdge = new EdgeLine(TmpVertexes[0], TmpVertexes[1], AnotherEdge);
										AddedEdge->set_color(EdgeHighlightedColor);
										EdgeDrawingQueue.push_back(AddedEdge);
										G.add_edge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText(), 0, true);
										if (AnotherEdge)
										{
											int i = 0;
											while (i < EdgeDrawingQueue.size() && (EdgeDrawingQueue[i]->get_start() != TmpVertexes[1] || EdgeDrawingQueue[i]->get_end() != TmpVertexes[0]))
											{
												i++;
											}
											EdgeDrawingQueue[i]->setAnotherEdge(AnotherEdge);
										}
									}
									else
									{
										BMessage.set_string(Window, "There is already an edge between this vertices.");
									}
								}
								else
								{
									BMessage.set_string(Window, "Graph is already full.");
								}
								break;
							default:
								if (EdgeDrawingQueue.size() < VertexDrawingQueue.size() * (VertexDrawingQueue.size() - 3) + VertexDrawingQueue.size() * 2)
								{
									if (!G.is_edge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText()))
									{
										bool AnotherEdge = G.is_edge(TmpVertexes[1]->getText(), TmpVertexes[0]->getText());
										AddedEdge = new EdgeLine(TmpVertexes[0], TmpVertexes[1], AnotherEdge);
										AddedEdge->set_color(EdgeHighlightedColor);
										EdgeDrawingQueue.push_back(AddedEdge);
										G.add_edge(TmpVertexes[0]->getText(), TmpVertexes[1]->getText(), 0, true);
										if (AnotherEdge)
										{
											int i = 0;
											while (i < EdgeDrawingQueue.size() && (EdgeDrawingQueue[i]->get_start() != TmpVertexes[1] || EdgeDrawingQueue[i]->get_end() != TmpVertexes[0]))
											{
												i++;
											}
											EdgeDrawingQueue[i]->setAnotherEdge(AnotherEdge);
										}
									}
									else
									{
										BMessage.set_string(Window, "There is already an edge between this vertices.");
									}
								}
								else
								{
									BMessage.set_string(Window, "Graph is already full.");
								}
								break;
							}

						}
					}

					if (SalesmanButton.contain(mousePos) && not_intersection(VertexDrawingQueue, mousePos))
					{
						if (SalesmanButton.Mode() == On)
						{
							if (VertexDrawingQueue.size() > 0) BMessage.set_string(Window, "Click and hold the LMB to move the vertex.\nClick RMB to edit a vertex or edge.");
							else BMessage.set_string(Window, "");
							SalesmanButton.setMode(Off);
							CheckVertecies(TmpVertexes);
							CheckEdges(TmpEdges);
							AnyButtonWasOn = false;
						}
						else if (SalesmanButton.Mode() == Off)
						{
							if (AnyButtonWasOn)
							{
								DeleteButton.setMode(Off);
								AddEdgeButton.setMode(Off);
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
							}
							BMessage.set_string(Window, "Click the LMB on the vertex to see Salesman route.");
							SalesmanButton.setMode(On);
							AnyButtonWasOn = true;
						}
					}
					else if (SalesmanButton.Mode() == On)
					{
						for (int i = 0; i < VertexDrawingQueue.size(); i++)
						{
							if (VertexDrawingQueue[i]->contain(mousePos))
							{
								if (VertexDrawingQueue[i]->get_color() == VertexDefaultColor)
								{
									if (TmpVertexes.size() == 0)
									{
										VertexDrawingQueue[i]->set_color(VertexHighlightedColor);
										TmpVertexes.push_back(VertexDrawingQueue[i]);
										if (G.is_weighted())
										{
											vector<GraphVertex<int>*> vroute = G.Salesman(VertexDrawingQueue[i]->getText());
											if (vroute.size() == VertexDrawingQueue.size() + 1)
											{
												string sroute = G.get_str_route(vroute);
												BMessage.set_string(Window, sroute);
											}
											else
											{
												BMessage.set_string(Window, "There's no way for Salesman.");
											}
										}
										else
										{
											BMessage.set_string(Window, "Not all edges have weight.");
										}
									}
								}
								else if (VertexDrawingQueue[i]->get_color() == VertexHighlightedColor)
								{
									if (TmpVertexes.size() == 1)
									{
										VertexDrawingQueue[i]->set_color(VertexDefaultColor);
										TmpVertexes.erase(vector<VertexCircle*>::iterator(TmpVertexes.begin()));
										BMessage.set_string(Window, "Click the LMB on the vertex to see Salesman route.");
									}
								}
							}
						}
					}

					if (!AnyButtonWasOn)
					{
						bool Continue = true;
						for (int i = VertexDrawingQueue.size() - 1; i >= 0 && Continue; i--)
						{
							if (VertexDrawingQueue[i]->contain(mousePos))
							{
								movingVert = VertexDrawingQueue[i];
								movingVert->set_color(VertexHighlightedColor);
								vector<VertexCircle*>::iterator it = VertexDrawingQueue.begin();
								for (it; it != VertexDrawingQueue.end() && *it != movingVert; it++) {}
								VertexDrawingQueue.erase(it);
								VertexDrawingQueue.push_back(movingVert);

								for (int i = 0; i < EdgeDrawingQueue.size(); i++)
								{
									if (EdgeDrawingQueue[i]->containVert(movingVert))
									{
										movingEdges.push_back(EdgeDrawingQueue[i]);
									}
								}
								Continue = false;
							}
						}
					}
				}

				else if (event.mouseButton.button == Mouse::Right)
				{
					bool Continue = true;

					for (int i = VertexDrawingQueue.size() - 1; i >= 0 && Continue; i--)
					{
						if (VertexDrawingQueue[i]->contain(mousePos))
						{
							if (AnyButtonWasOn)
							{
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								SalesmanButton.setMode(Off);
								AddEdgeButton.setMode(Off);
								DeleteButton.setMode(Off);
								AnyButtonWasOn = false;
							}
							Window.clear(WindowColor);

							DrawEdges(Window, EdgeDrawingQueue);
							BMessage.set_string(Window, "Click and hold the LMB to move the vertex\nClick RMB to edit a vertex or edge");
							DrawButtons(Window, 4, AddVertexButton, AddEdgeButton, DeleteButton, SalesmanButton);
							VertexDrawingQueue[i]->set_color(VertexHighlightedColor);
							DrawVertexes(Window, VertexDrawingQueue);
							Window.display();

							VertexSettingsWindow(VertexDrawingQueue[i], G);
							
							Continue = false;
							VertexDrawingQueue[i]->set_color(VertexDefaultColor);
						}
					}

					for (int i = 0; i < EdgeDrawingQueue.size() && Continue; i++)
					{
						if (EdgeDrawingQueue[i]->contain(mousePos))
						{
							if (AnyButtonWasOn)
							{
								CheckVertecies(TmpVertexes);
								CheckEdges(TmpEdges);
								SalesmanButton.setMode(Off);
								AddEdgeButton.setMode(Off);
								DeleteButton.setMode(Off);
								AnyButtonWasOn = false;
							}

							EdgeDrawingQueue[i]->set_color(EdgeHighlightedColor);

							Window.clear(WindowColor);

							BMessage.set_string(Window, "Click and hold the LMB to move the vertex\nClick RMB to edit a vertex or edge");
							DrawEdges(Window, EdgeDrawingQueue);
							DrawButtons(Window, 4, AddVertexButton, AddEdgeButton, DeleteButton, SalesmanButton);
							DrawVertexes(Window, VertexDrawingQueue);

							Window.display();

							EdgeSettingsWindow(EdgeDrawingQueue[i], G, EdgeDrawingQueue);

							Continue = false;
							if (EdgeDrawingQueue[i] != nullptr) EdgeDrawingQueue[i]->set_color(EdgeDefaultColor);
							else
							{
								vector<EdgeLine*>::iterator del;
								del = EdgeDrawingQueue.begin() + i;
								EdgeDrawingQueue.erase(del);
							}
						}
					}
				}
				break;

			case Event::MouseMoved:
				if (movingVert != nullptr)
				{
					MoveVertex(Window, movingVert, movingEdges, mousePos, prev_mousePos);
				}
				break;

			case Event::MouseButtonReleased:
				if (movingVert != nullptr)
				{
					movingVert->set_color(VertexDefaultColor);
					movingVert = nullptr;
					movingEdges.clear();
				}

				if (AddVertexButton.Mode() == On)
				{
					AddVertexButton.setMode(Off);
					AddedVertex->set_color(VertexDefaultColor);
					AddedVertex = nullptr;
					AnyButtonWasOn = false;
				}
				else if (AddEdgeButton.Mode() == On && ApplyButton.Mode() == On)
				{
					VertexesDefaultColor(TmpVertexes);
					if (AddedEdge != nullptr)
					{
						AddedEdge->set_color(EdgeDefaultColor);
						AddedEdge = nullptr;
					}
					TmpVertexes.clear();
				}

				ApplyButton.setMode(Off);
				if (TmpVertexes.size() == 0 && TmpEdges.size() == 0) MustShowApply = false;
				break;

			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape)
				{
					if (VertexDrawingQueue.size() > 0) BMessage.set_string(Window, "Click and hold the LMB to move the vertex\nClick RMB to edit a vertex or edge");
					else BMessage.set_string(Window, "");

					if (AnyButtonWasOn)
					{
						AddEdgeButton.setMode(Off);
						DeleteButton.setMode(Off);
						SalesmanButton.setMode(Off);
						CheckVertecies(TmpVertexes);
						CheckEdges(TmpEdges);
						AnyButtonWasOn = false;
					}
					
					else Window.close();
				}
				break;

			case Event::Resized: 
				SetResize(Window, prevWpos, prevsize, AddVertexButton, AddEdgeButton, DeleteButton, ApplyButton, SalesmanButton, BMessage, VertexDrawingQueue, EdgeDrawingQueue);
				break;
			}
		}
		prev_mousePos = mousePos;
	}

	for (int i = 0; i < VertexDrawingQueue.size(); i++)
	{
		delete VertexDrawingQueue[i];
	}

	for (int i = 0; i < EdgeDrawingQueue.size(); i++)
	{
		delete EdgeDrawingQueue[i];
	}

	VertexDrawingQueue.clear();
	EdgeDrawingQueue.clear();

	return 0;
}