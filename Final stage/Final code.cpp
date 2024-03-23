#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

using namespace std;

// Base class for gates with multiple inputs
class MultiInputGate {
protected:
    vector<int> inputs;

public:
    // Constructor to initialize inputs vector with specified size
    MultiInputGate(size_t numInputs) : inputs(numInputs, 0) {}

    // Function to set input at a specific index
    void setInput(size_t index, int value) {
        if (index < inputs.size()) {
            inputs[index] = value;
        }
    }

    // Virtual function for evaluating gate's output
    virtual int evaluate() const = 0;

    // Virtual destructor
    virtual ~MultiInputGate() {}
};

// Structure to store gate information
struct Gate {
    string name;
    MultiInputGate* gatePtr;

    // Constructor
    Gate(const string& n, MultiInputGate* g) : name(n), gatePtr(g) {}

    // Method to get gate's name
    string getName() const {
        return name;
    }

    // Virtual destructor
    virtual ~Gate() {
        delete gatePtr;
    }
};

// NOT gate class
class NOTGate : public MultiInputGate {
private:
    int input;

public:
    // Constructor to initialize input
    NOTGate() : MultiInputGate(1), input(0) {}

    // Function to set input
    void setInput(int value) {
        input = value;
    }

    // Function to evaluate NOT gate's output
    int evaluate() const override {
        // If input is 0, return 1; if input is 1, return 0
        return (input == 0) ? 1 : 0;
    }
};

// AND gate class
class ANDGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    ANDGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate AND gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }
        // Iterate through inputs and return 0 if any input is 0, otherwise return 1
        for (int input : inputs) {
            if (input == 0) {
                return 0;
            }
        }
        return 1;
    }
};

// Function to parse the library file and create gate objects
vector<Gate> parseLibrary(const string& filename) {
    vector<Gate> gates;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open library file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, type, expression;
        int delay;
        ss >> name >> type >> expression >> delay;

        // Create gate objects based on gate type
        MultiInputGate* gatePtr = nullptr;
        if (type == "NOT") {
            gatePtr = new NOTGate();
        }
        else if (type == "AND2") {
            gatePtr = new ANDGate(2);
        }

        if (gatePtr) {
            gates.emplace_back(name, gatePtr);
        }
    }
    file.close();
    return gates;
}

// Function to parse the circuit file and determine gate connections
vector<tuple<string, vector<string>>> parseCircuit(const string& circuitFile) {
    vector<tuple<string, vector<string>>> connections;
    ifstream file(circuitFile);
    if (!file.is_open()) {
        cerr << "Failed to open circuit file: " << circuitFile << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    bool inInputs = false;
    while (getline(file, line)) {
        if (line == "INPUTS:") {
            inInputs = true;
        }
        else if (line == "COMPONENTS:") {
            inInputs = false;
        }
        else if (!line.empty()) {
            if (inInputs) {
                stringstream ss(line);
                string inputName;
                ss >> inputName;
                connections.emplace_back(inputName, vector<string>());
            }
            else {
                stringstream ss(line);
                string gateName, gateType, output, input;
                ss >> gateName >> gateType >> output;
                vector<string> inputs;
                while (ss >> input) {
                    inputs.push_back(input);
                }
                connections.emplace_back(output, inputs);
            }
        }
    }
    file.close();
    return connections;
}

// Function to parse the stimuli file and output its content
vector<tuple<int, string, int>> parseStimuli(const string& stimuliFile) {
    vector<tuple<int, string, int>> stimuli;
    ifstream file(stimuliFile);
    if (!file.is_open()) {
        cerr << "Error: Unable to open stimuli file '" << stimuliFile << "'." << endl;
        return stimuli;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int time_stamp;
        char   comma1, comma2;
        string input;
        int logic_value;
        ss >> time_stamp >> comma1 >> input >> comma2 >> logic_value;
        if (ss.fail() || comma1 != ',') {
            cerr << "Error parsing stimuli file: " << line << endl;
        }
        else {
            stimuli.emplace_back(time_stamp, input, logic_value);
        }
    }

    file.close();
    return stimuli;
}

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

// ...
// Same as the provided code
// ...

int main() {
    string libraryFile = "library.lib";
    string circuitFile = "circuit.cir";
    string stimuliFile = "stimuli.stim";

    // Parse the library file to create gate objects
    vector<Gate> gates = parseLibrary(libraryFile);

    // Parse the circuit file and determine gate connections
    vector<tuple<string, vector<string>>> connections = parseCircuit(circuitFile);

    // Parse the stimuli file and store stimuli data
    vector<tuple<int, string, int>> stimuli = parseStimuli(stimuliFile);

    // Define the current time stamp
    int current_time = 0;

    // Evaluate gate outputs for initial time step
    cout << "Time: 0 ns" << endl;
    for (const auto& gate : gates) {
        // Manually evaluate gate's output
        int output = gate.gatePtr->evaluate();
        cout << gate.getName() << " Output: " << output << endl;
    }
    cout << endl;

    // Iterate over stimuli and simulate the circuit behavior
    for (const auto& stimulus : stimuli) {
        int time_stamp = get<0>(stimulus);
        string input = get<1>(stimulus);
        int logic_value = get<2>(stimulus);

        // Process time steps until reaching the current time stamp
        while (current_time < time_stamp) {
            // Increment the current time by 200 ns
            current_time += 200;

            // Evaluate gate outputs for the current time step
            cout << "Time: " << current_time << " ns" << endl;
            for (const auto& gate : gates) {
                // Manually evaluate gate's output
                int output = gate.gatePtr->evaluate();
                cout << gate.getName() << " Output: " << output << endl;
            }
            cout << endl;
        }

        // Update gate inputs according to the stimuli
        for (const auto& gate : gates) {
            for (const auto& connection : connections) {
                if (gate.getName() == get<0>(connection)) {
                    vector<string> inputs = get<1>(connection);
                    for (size_t i = 0; i < inputs.size(); ++i) {
                        if (inputs[i] == input) {
                            gate.gatePtr->setInput(i, logic_value);
                        }
                    }
                }
            }
        }
    }

    // Release memory allocated for gate objects
    for (const auto& gate : gates) {
        delete gate.gatePtr;
    }

    return 0;
}
