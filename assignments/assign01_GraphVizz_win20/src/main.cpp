/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <chrono>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
void read_file(ifstream& file, string& filename);
void read_time(int& runTime);
void createNodeAndEdge(ifstream& file, SimpleGraph& graph, const int& n);
void calKattract(SimpleGraph& graph, vector<pair<double, double>>& delta_pos);
void calKepel(SimpleGraph& graph, int& n, vector<pair<double, double>>& delta_pos);
void updatePos(SimpleGraph& graph, int& n, vector<pair<double, double>>& delta_pos);


// Main method
int main() {
    Welcome();
    /* TODO: your implementation here */
    ifstream file;
    string filename;
    int n;  // the number of nodes
    SimpleGraph graph;
    int runTime;

    while (true) {
        // Prompt the user for the name of a file that contains the graph data,
        // and open the file. If unsuccessful, reprompt.
        read_file(file, filename);

        // Prompt the user for the number of microseconds to run the algorithm,
        // which should be a positive integer. If the input is invalid, reprompt.
        read_time(runTime);

        // Place all the nodes in a circle.
        file >> n;
        createNodeAndEdge(file, graph, n);

        InitGraphVisualizer(graph);
        DrawGraph(graph);

        int milliseconds = 0;
        // retrieve current times
        auto startTime = std::chrono::high_resolution_clock::now();


        for (int i = 0; i < runTime; i++) {
            // retrieve current times
            auto endTime = std::chrono::high_resolution_clock::now();

            // calculate elapsed time, and convert to milliseconds
            auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            if (i % 10 == 0) cout << i << " iteration" << endl;

            DrawGraph(graph);

            vector<pair<double, double>> delta_pos(n, make_pair(0, 0));
            calKattract(graph, delta_pos);
            calKepel(graph, n, delta_pos);
            updatePos(graph, n, delta_pos);
        }

        cout << "Do you want to try another file? (yes/no): ";
        string ans;
        getline(cin, ans);
        if (ans != "yes") {
            break;
        }
    }

    return 0;
}


void read_file(ifstream& file, string& filename) {
    while (true) {
        cout << "Input the name of file: ";
        getline(cin, filename);

        file.open(filename);
        if (file.is_open()) {
            break;
        }
        cout << "Couldn't open file. Please try again." << endl;
    }
}

void read_time(int& runTime) {
    while (true) {
        cout << "Input the number of microseconds to run the algorithm: ";
        string line; char trash;
        if (!getline(cin, line)) {
            cout << "Input invalid! Please try again." << endl;
        }
        istringstream iss(line);
        if (iss >> runTime && !(iss >> trash)) {
            if (runTime > 0) {
                break;
            }
        }
        cout << "Input invalid! Please try again." << endl;
    }
}

void createNodeAndEdge(ifstream& file, SimpleGraph& graph, const int& n) {
    // Place all the nodes in a circle.
    const double kPi = 3.14159265358979323;
    for (int k = 0; k < n; k++) {
        double angle = 2 * kPi * k / n;
        double x = cos(angle);
        double y = sin(angle);
        graph.nodes.push_back(Node{x, y});
    }

    // Add all the edges to your graph.
    size_t start, end;
    while (file >> start >> end) {
        graph.edges.push_back(Edge{start, end});
    }
    file.close();
}

void calKattract(SimpleGraph& graph, vector<pair<double, double>>& delta_pos) {
    const double kattract = 0.001;
    for (auto it : graph.edges) {
        size_t first = it.start;
        size_t end = it.end;

        auto [x0, y0] = graph.nodes.at(first);
        auto [x1, y1] = graph.nodes.at(end);

        // Compute Fattract
        double fattract = kattract * ((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));
        double theta = atan2(y1 - y0, x1 - x0);
        delta_pos.at(first).first += fattract * cos(theta);
        delta_pos.at(first).second += fattract * sin(theta);
        delta_pos.at(end).first -= fattract * cos(theta);
        delta_pos.at(end).second -= fattract * sin(theta);
    }
}

void calKepel(SimpleGraph& graph, int& n, vector<pair<double, double>>& delta_pos) {
    const double krepel = 0.001;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            auto [x0, y0] = graph.nodes.at(i);
            auto [x1, y1] = graph.nodes.at(j);

            // Compute Frepel
            double frepel = krepel / sqrt((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));
            double theta = atan2(y1 - y0, x1 - x0);
            delta_pos.at(i).first -= frepel * cos(theta);
            delta_pos.at(i).second -= frepel * sin(theta);
            delta_pos.at(j).first += frepel * cos(theta);
            delta_pos.at(j).second += frepel * sin(theta);
        }
    }
}

void updatePos(SimpleGraph& graph, int& n, vector<pair<double, double>>& delta_pos) {
    for (size_t i = 0; i < n; i++) {
        graph.nodes[i].x += delta_pos[i].first;
        graph.nodes[i].y += delta_pos[i].second;
    }
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}



