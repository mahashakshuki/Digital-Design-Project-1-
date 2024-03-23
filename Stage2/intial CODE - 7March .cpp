#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Component {
    string name;
    string type;
    vector<string> inputs;
    string output;
};

int main() {
    ifstream file("input.txt");
    string line;
    vector<string> inputs;
    vector<Component> components;

    enum State { INPUTS, COMPONENTS };
    State state = INPUTS;

    while (getline(file, line)) {
        if (line.empty())
            continue;

        if (line == "INPUTS:") {
            state = INPUTS;
            continue;
        } else if (line == "COMPONENTS:") {
            state = COMPONENTS;
            continue;
        }

        stringstream ss(line);
        string token;

        switch (state) {
            case INPUTS:
                while (ss >> token) {
                    inputs.push_back(token);
                }
                break;
            case COMPONENTS:
                Component component;
                ss >> component.name >> component.type >> component.output;
                while (ss >> token) {
                    component.inputs.push_back(token);
                }
                components.push_back(component);
                break;
        }
    }

    cout << "Inputs:" << endl;
    for (const auto& input : inputs) {
        cout << input << endl;
    }

    cout << "Components:" << endl;
    for (const auto& component : components) {
        cout << "Name: " << component.name << ", Type: " << component.type << ", Output: " << component.output << ", Inputs: ";
        for (const auto& input : component.inputs) {
            cout << input << " ";
        }
        cout << endl;
    }

    return 0;
}
