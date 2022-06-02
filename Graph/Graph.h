#pragma once
#include <string>
#include <vector>
#include <queue>
#include <iterator>
#include <iomanip>
#include "usingstd.h"

template <class T>
const T Infinity = 1000000000;

template <class T>
class Graph;

template <class T>
class GraphVertex
{
	friend class Graph<T>;
private:
	string name;
	T vertex_weight;

public:
	GraphVertex(string name, T vertex_weight = NULL)
	{
		this->name = name;
		this->vertex_weight = vertex_weight;
	}
	string get_name()
	{
		return name;
	}
};

template <class T>
class Graph
{
protected:
	vector<vector<T>> matrix;
	vector<GraphVertex<T>> vertexes;

	int get_width();
	void print_str(const int& width);
	void continue_depth_first_search(const string& current_vertex, vector<bool>& visited);
	void fill_weights(const string& start_vertex);
	T continue_Salesman(const string& start_vertex, const string& current_vertex, vector<bool> visited, vector<GraphVertex<T>*>& route, const T& prev_route);
public:
	Graph(){}
	~Graph()
	{
		for (int i = 0; i < vertexes.size(); i++)
		{
			matrix[i].clear();
		}
		matrix.clear();
		vertexes.clear();
	}
	void add_vertex(const string& name, const T& vertex_weight = NULL);
	void add_edge(const string& start_vertex, const string& end_vertex, const T& edge_weight = 0, const bool& direction = false);

	bool is_vertex(const string& name);

	void print_matrix();

	void delete_vertex(const string& name);
	void delete_edge(const string& start_vertex, const string& end_vertex);

	void set_edge_weight(const string& start_vertex, const string& end_vertex, const T& edge_weight = 0, const bool& direction = false);
	void change_edge_direction(const string& start_vertex, const string& end_vertex);
	void swap_two_edges(const string& start_vertex, const string& end_vertex);

	bool is_edge(const string& start_vertex, const string& end_vertex);
	bool is_two_edges(const string& start_vertex, const string& end_vertex);

	void set_vertex_weight(const string& name, const T& new_vertex_weight = 0);
	void set_vertex_name(const string& name, const string& new_name = "");

	bool is_empty();

	void depth_first_search(const string& start_vertex);

	vector<GraphVertex<T>*> get_neighbours(const string& start_vertex);
	int get_vertex_index(const string& name);

	void breadth_first_search(const string& start_vertex);

	bool is_weighted();

	void Dijkstra(const string& start_vertex);
	GraphVertex<T>* find_not_visited(const vector<bool>& visited, const string& current_vertex);
	bool all_visited(const vector<bool>& visited);

	vector<GraphVertex<T>*> Salesman(const string& start_vertex);
	vector<GraphVertex<T>*> what_neighbour(const string& start_vertex);
	void print_route(const vector<GraphVertex<T>*>& route);
	string get_str_route(const vector<GraphVertex<T>*>& route);
};

template <class T>
void Graph<T>::add_vertex(const string& name, const T& vertex_weight)
{
	vertexes.push_back(GraphVertex<T> (name, vertex_weight));
	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i].push_back(-1);
	}
	matrix.push_back(vector<T>(vertexes.size(), -1));
}

template <class T>
void Graph<T>::add_edge(const string& start_vertex, const string& end_vertex, const T& edge_weight, const bool& direction)
{
	int i = get_vertex_index(start_vertex);
	int j = get_vertex_index(end_vertex);
	if (direction) matrix[i][j] = edge_weight;
	else matrix[i][j] = matrix[j][i] = edge_weight;
}

template <class T>
bool Graph<T>::is_vertex(const string& name)
{
	return get_vertex_index(name) != -1;
}

template <class T>
void Graph<T>::print_str(const int& width)
{
	for (int i = 0; i < vertexes.size() + 1; i++)
	{
		for (int j = 0; j < width + 1; j++)
		{
			cout << "-";
		}
	}
	cout << endl;
}

template <class T>
void Graph<T>::print_matrix()
{
	if (!is_empty())
	{
		cout << "Матрица смежности графа:\n";
		cout << "'число' - вес ребра\n'#' - вершины в этом направлении не связаны\n'?' - вершины связаны, но вес ребра неизвестен\n\n";
		int width = get_width();
		print_str(width);
		for (int i = 0; i < width; i++)
		{
			cout << " ";
		}
		cout << "|";
		for (int i = 0; i < vertexes.size(); i++)
		{
			cout << setw(width) << vertexes[i].name << "|";
		}
		cout << endl;
		print_str(width);
		for (int i = 0; i < vertexes.size(); i++)
		{
			cout << vertexes[i].name;
			for (int l = 0; l < width - vertexes[i].name.size(); l++)
			{
				cout << " ";
			}
			cout << "|";
			for (int j = 0; j < vertexes.size(); j++)
			{
				if (matrix[i][j] == -1) cout << setw(width) << "#";
				else
					if (matrix[i][j] == 0) cout << setw(width) << "?";
					else cout << setw(width) << setprecision(width) << matrix[i][j];
				cout << "|";
			}
			cout << endl;
			print_str(width);
		}
	}
	else
	{
		cout << "В графе нет вершин.\n";
	}
	cout << endl;
}

template <class T>
int Graph<T>::get_width()
{
	int max = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name.size() > max) max = vertexes[i].name.size();
	}
	if (typeid(T) == typeid(int&))
	{
		for (int i = 0; i < vertexes.size(); i++)
		{
			for (int j = 0; j < vertexes.size(); j++)
			{
				if (to_string(matrix[i][j]).size() > max) max = to_string(matrix[i][j]).size();
			}
		}
	}
	else
	{
		for (int i = 0; i < vertexes.size(); i++)
		{
			for (int j = 0; j < vertexes.size(); j++)
			{
				if (to_string(int(matrix[i][j])+3).size() > max) 
					max = to_string(matrix[i][j]).size();
			}
		}
	}
	return max;
}

template <class T>
void Graph<T>::delete_vertex(const string& name)
{
	vector<GraphVertex<T>>::template iterator v = vertexes.begin();
	vector<vector<T>>::template iterator m = matrix.begin();
	GraphVertex<T> vrtx = *v;
	int k = 0;
	while (vrtx.name != name)
	{
		v++;
		m++;
		k++;
		vrtx = *v;
	}
	vertexes.erase(v);
	matrix.erase(m);
	for (int i = 0; i < vertexes.size(); i++)
	{
		vector<T>::template iterator m1 = matrix[i].begin();
		for (int j = 0; j < k; j++)
		{
			m1++;
		}
		matrix[i].erase(m1);
	}
}

template <class T>
void Graph<T>::delete_edge(const string& start_vertex, const string& end_vertex)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	//if (matrix[i_1][i_2] == matrix[i_2][i_1]) matrix[i_1][i_2] = matrix[i_2][i_1] = -1; //if edge is non-direction deleted from 1 to 2 and from 2 to 1
	/*else*/  matrix[i_1][i_2] = -1;//if edge have direction delete only from 1 to 2 
}

template <class T>
void Graph<T>::set_edge_weight(const string& start_vertex, const string& end_vertex, const T& edge_weight, const bool& direction)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	if (!direction) matrix[i_1][i_2] = matrix[i_2][i_1] = edge_weight;
	else matrix[i_1][i_2] = edge_weight;
}

template <class T>
void Graph<T>::change_edge_direction(const string& start_vertex, const string& end_vertex)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	if (matrix[i_1][i_2] == matrix[i_2][i_1])
	{
		matrix[i_2][i_1] = -1;//set direction from 1 to 2
	}
	else//swap direction: from 1 to 2 -> from 2 to 1
	{
		matrix[i_2][i_1] = matrix[i_1][i_2];
		matrix[i_1][i_2] = -1;
	}
}

template <class T>
void Graph<T>::swap_two_edges(const string& start_vertex, const string& end_vertex)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	T tmp = matrix[i_2][i_1];
	matrix[i_2][i_1] = matrix[i_1][i_2];
	matrix[i_1][i_2] = tmp;
}

template <class T>
bool Graph<T>::is_edge(const string& start_vertex, const string& end_vertex)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	return matrix[i_1][i_2] != -1 /*|| matrix[i_2][i_1] != -1*/;
}

template <class T>
bool Graph<T>::is_two_edges(const string& start_vertex, const string& end_vertex)
{
	int i_1 = 0;
	int i_2 = 0;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex) i_1 = i;
		else if (vertexes[i].name == end_vertex) i_2 = i;
	}
	return matrix[i_1][i_2] != matrix[i_2][i_1] && matrix[i_1][i_2] != -1 && matrix[i_2][i_1] != -1;
}

template <class T>
void Graph<T>::set_vertex_weight(const string& name, const T& new_vertex_weight)
{
	int i = get_vertex_index(name);
	vertexes[i].vertex_weight = new_vertex_weight;
}

template <class T>
void Graph<T>::set_vertex_name(const string& name, const string& new_name)
{
	int i = 0;
	while (vertexes[i].name != name && i < vertexes.size())
	{
		i++;
	}
	vertexes[i].name = new_name;
}

template <class T>
bool Graph<T>::is_empty()
{
	return vertexes.size() == 0;
}

template <class T>
void Graph<T>::depth_first_search(const string& start_vertex)
{
	cout << "Вершина " << start_vertex << " с весом " << vertexes[get_vertex_index(start_vertex)].vertex_weight << " посещена\n";
	vector<bool> visited(vertexes.size());
	fill(visited.begin(), visited.end(), false);
	visited[get_vertex_index(start_vertex)] = true;
	vector<GraphVertex<T>*> neighbours = get_neighbours(start_vertex);
	for (int i = 0; i < neighbours.size(); i++)
	{
		if (!visited[get_vertex_index(neighbours[i]->name)])
		    continue_depth_first_search(neighbours[i]->name, visited);
	}
}

template <class T>
void Graph<T>::continue_depth_first_search(const string& current_vertex, vector<bool>& visited)
{
	cout << "Вершина " << current_vertex << " с весом " << vertexes[get_vertex_index(current_vertex)].vertex_weight << " посещена\n";
	visited[get_vertex_index(current_vertex)] = true;
	vector<GraphVertex<T>*> neighbours = get_neighbours(current_vertex);
	for (int i = 0; i < neighbours.size(); i++)
	{
		if (!visited[get_vertex_index(neighbours[i]->name)])
			continue_depth_first_search(neighbours[i]->name, visited);
	}
}

template <class T>
vector<GraphVertex<T>*> Graph<T>::get_neighbours(const string& start_vertex)
{
	int i = get_vertex_index(start_vertex);
	vector<GraphVertex<T>*> neighbours;
	for (int j = 0; j < vertexes.size(); j++)
	{
		if (matrix[i][j] != -1)
		{
			neighbours.push_back(&vertexes[j]);
		}
	}
	return neighbours;
}

template <class T>
int Graph<T>::get_vertex_index(const string& name)
{
	int i = vertexes.size() - 1;
	bool f = vertexes[i].name == name;
	while (!f && i > 0)
	{
		i--;
		f = vertexes[i].name == name;
	}
	if (f) return i;
	else return -1;
}

template <class T>
void Graph<T>::breadth_first_search(const string& start_vertex)
{
	vector<bool> visited(vertexes.size());
	fill(visited.begin(), visited.end(), false);
	queue<GraphVertex<T>*> q;

	q.push(&vertexes[get_vertex_index(start_vertex)]);
	while (!q.empty())
	{
		GraphVertex<T> current_vertex = *q.front();
		cout << "Вершина " << current_vertex.name << " с весом " << current_vertex.vertex_weight << " посещена\n";
		visited[get_vertex_index(current_vertex.name)] = true;
		q.pop();
		vector<GraphVertex<T>*> neighbours = get_neighbours(current_vertex.name);
		for (int i = 0; i < neighbours.size(); i++)
		{
			if (!visited[get_vertex_index(neighbours[i]->name)])
			{
				visited[get_vertex_index(neighbours[i]->name)] = true;
				q.push(neighbours[i]);
			}
		}
	}
}

template <class T>
bool Graph<T>::is_weighted()
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (matrix[i][j] == 0) return false;
		}
	}
	return true;
}

template <class T>
void Graph<T>::Dijkstra(const string& start_vertex)
{
	vector<bool> visited(vertexes.size());
	fill(visited.begin(), visited.end(), false);
	fill_weights(start_vertex);

	int current_index = get_vertex_index(start_vertex);
	GraphVertex<T>* current_vertex = &vertexes[current_index];

	while (!all_visited(visited))
	{
		GraphVertex<T>* min_neighbour = nullptr;
		T min_weight = Infinity<T>;

		vector<GraphVertex<T>*> neighbours = get_neighbours(current_vertex->name);

		for (int i = 0; i < neighbours.size(); i++)
		{
			int neighbour_index = get_vertex_index(neighbours[i]->name);
			T edge_weight = matrix[current_index][neighbour_index];

			if (current_vertex->vertex_weight + edge_weight < neighbours[i]->vertex_weight)
			{
				neighbours[i]->vertex_weight = current_vertex->vertex_weight + edge_weight;
			}
			if (!visited[neighbour_index] && min_weight > edge_weight)
			{
				min_weight = neighbours[i]->vertex_weight;
				min_neighbour = neighbours[i];
			}
		}

		visited[current_index] = true;
		if (min_neighbour != nullptr)
		{
			current_vertex = min_neighbour;
			current_index = get_vertex_index(current_vertex->name);
		}
		else
		{
			if (!all_visited(visited))
			{
				current_vertex = find_not_visited(visited, start_vertex);
				current_index = get_vertex_index(current_vertex->name);
			}
		}
	}
}

template <class T>
GraphVertex<T>* Graph<T>::find_not_visited(const vector<bool>& visited, const string& current_vertex)
{
	vector<GraphVertex<T>*> neighbours = get_neighbours(current_vertex);
	bool f = false;
	for (int i = 0; i < neighbours.size() && !f; i++)
	{
		f = !visited[get_vertex_index(neighbours[i]->name)];
		if (f) return neighbours[i];
	}

	if (!f)
	{
		if (neighbours.size() == 0) return nullptr;
		GraphVertex<T>* V;
		for (int i = 0; i < neighbours.size() && !f; i++)
		{
			V = find_not_visited(visited, neighbours[i]->name);
			if (V != nullptr) return V;
		}
	}
}

template <class T>
void Graph<T>::fill_weights(const string& start_vertex)
{
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (vertexes[i].name == start_vertex)
		{
			vertexes[i].vertex_weight = 0;
		}
		else
		{
			vertexes[i].vertex_weight = Infinity<T>;
		}
	}
}

template <class T>
bool Graph<T>::all_visited(const vector<bool>& visited)
{
	for (int i = 0; i < visited.size(); i++)
	{
		if (visited[i] == false) return false;
	}
	return true;
}

template <class T>
vector<GraphVertex<T>*> Graph<T>::Salesman(const string& start_vertex)
{
	vector<GraphVertex<T>*> route;
	vector<GraphVertex<T>*> neighbours = what_neighbour(start_vertex);
	int start_index = get_vertex_index(start_vertex);
	vector<bool> visited(vertexes.size());
	fill(visited.begin(), visited.end(), false);
	visited[start_index] = true;
	for (int i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].vertex_weight = Infinity<T>;
	}

	for (int i = 0; i < neighbours.size(); i++)
	{
		int neighbour_index = get_vertex_index(neighbours[i]->name);
		T edge_weight = matrix[neighbour_index][start_index];
		T neighbour_weight = continue_Salesman(start_vertex, neighbours[i]->name, visited, route, edge_weight);
		
		if (neighbour_weight + edge_weight <= vertexes[start_index].vertex_weight)
		{
			route.push_back(neighbours[i]);
		}
	}
	route.push_back(&vertexes[start_index]);
	return route;
}

template <class T>
T Graph<T>::continue_Salesman(const string& start_vertex, const string& current_vertex, vector<bool> visited, vector<GraphVertex<T>*>& route, const T& prev_route)
{
	if (prev_route >= vertexes[get_vertex_index(start_vertex)].vertex_weight) return Infinity<T>;
	vector<GraphVertex<T>*> neighbours = what_neighbour(current_vertex);
	int current_index = get_vertex_index(current_vertex);
	visited[current_index] = true;
	vertexes[current_index].vertex_weight = Infinity<T>;
	T tmp = vertexes[current_index].vertex_weight;

	for (int i = 0; i < neighbours.size(); i++)
	{
		int neighbour_index = get_vertex_index(neighbours[i]->name);
		T edge_weight = matrix[neighbour_index][current_index];
		if (neighbours[i]->name == start_vertex)
		{
			if (all_visited(visited))
			{
				if (prev_route + edge_weight < vertexes[get_vertex_index(start_vertex)].vertex_weight)
				{
					vertexes[get_vertex_index(start_vertex)].vertex_weight = prev_route + edge_weight;
					vertexes[current_index].vertex_weight = edge_weight;
					route.clear();
					route.push_back(neighbours[i]);
					return edge_weight;
				}
			}
			else
			{
				if (neighbours.size() == 1)
				{
					return Infinity<T>;
				}
			}
		}
		else
		{
			if (!visited[neighbour_index])
			{
				T neighbour_weight = continue_Salesman(start_vertex, neighbours[i]->name, visited, route, prev_route + edge_weight);
				if (neighbour_weight + edge_weight < vertexes[current_index].vertex_weight)
				{
					route.push_back(neighbours[i]);
					vertexes[current_index].vertex_weight = neighbour_weight + edge_weight;
				}
			}
		}
	}

	return vertexes[current_index].vertex_weight;
}

template <class T>
vector<GraphVertex<T>*> Graph<T>::what_neighbour(const string& start_vertex)
{
	int j = get_vertex_index(start_vertex);
	vector<GraphVertex<T>*> neighbours;
	for (int i = 0; i < vertexes.size(); i++)
	{
		if (matrix[i][j] > 0) neighbours.push_back(&vertexes[i]);
	}
	return neighbours;
}

template <class T>
void Graph<T>::print_route(const vector<GraphVertex<T>*>& route)
{
	cout << route[0]->name;
	for (int i = 1; i < route.size(); i++)
	{
		cout << " -> " << route[i]->name;
	}
	cout << endl << matrix[get_vertex_index(route[0]->name)][get_vertex_index(route[1]->name)];
	for (int i = 1; i < route.size() - 1; i++)
	{
		cout << " + " << matrix[get_vertex_index(route[i]->name)][get_vertex_index(route[i + 1]->name)];
	}
	cout << " = " << route[0]->vertex_weight;
}

template <class T>
string Graph<T>::get_str_route(const vector<GraphVertex<T>*>& route)
{
	string r = route[0]->name;
	for (int i = 1; i < route.size(); i++)
	{
		r += " > " + route[i]->name;
	}
	r += "\n" + to_string(matrix[get_vertex_index(route[0]->name)][get_vertex_index(route[1]->name)]);
	for (int i = 1; i < route.size() - 1; i++)
	{
		r += " + " + to_string(matrix[get_vertex_index(route[i]->name)][get_vertex_index(route[i + 1]->name)]);
	}
	r += " = " + to_string(route[0]->vertex_weight);
	return r;
}