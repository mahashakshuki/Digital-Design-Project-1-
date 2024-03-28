#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map> 
#include <algorithm>
using namespace std;

// Base class for gates with multiple inputs
class MultiInputGate {
public:
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
    int delay;
    MultiInputGate* gatePtr;

    // Constructor
    Gate(const string& n, const int& d, MultiInputGate* gateptr) : name(n), delay(d), gatePtr(gateptr) {}

    // Method to get gate's name
    string getName() const {
        return name;
    }

    // Method to get gate's delay
    int getDelay() const {
        return delay;
    }
};

// NOT gate class
class NOTGate : public MultiInputGate {
public:
    // Constructor to initialize input
    NOTGate() : MultiInputGate(1) {}

    // Function to set input
    void setInput(int value) {
        inputs[0] = value;
    }

    // Function to evaluate NOT gate's output
    int evaluate() const override {
        // If input is 0, return 1; if input is 1, return 0
        return (inputs[0] == 0) ? 1 : 0;
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

// OR gate class
class ORGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    ORGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate OR gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

        // Iterate through inputs and return 1 if any input is 1, otherwise return 0
        for (int input : inputs) {
            if (input == 1) {
                return 1;
            }
        }
        return 0;
    }
};

// XOR gate class
class XORGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    XORGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate XOR gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

        // Count the number of 1s in the inputs
        int countOnes = 0;
        for (int input : inputs) {
            countOnes += input;
        }

        // Return 1 if the count of 1s is odd, otherwise return 0
        return countOnes % 2 == 1 ? 1 : 0;
    }
};

// NAND gate class
class NANDGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    NANDGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate NAND gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

        // Iterate through inputs and return 1 if any input is 0, otherwise return 0
        for (int input : inputs) {
            if (input == 0) {
                return 1;
            }
        }
        return 0;
    }
};

// NOR gate class
class NORGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    NORGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate NOR gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

        // Iterate through inputs and return 0 if any input is 1, otherwise return 1
        for (int input : inputs) {
            if (input == 1) {
                return 0;
            }
        }
        return 1;
    }
};

// Function to parse the library file and create gate objects
vector<tuple<string, int, int>> parseLibrary(const string& filename) {
    vector<tuple<string, int, int>> librarygates;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open library file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string Component_Name, expression, comma;
        int delay, numInputs;

        // Parse each line of the library file
        ss >> Component_Name >> numInputs >> comma >> expression >> delay;
        Component_Name.pop_back(); // Removing comma
        expression.pop_back();    // Removing comma

        // Create a tuple to store gate information and add it to the library vector
        librarygates.emplace_back(Component_Name, delay, numInputs);
    }
    file.close();
    return librarygates;
}

// Function to parse the circuit file and determine gate connections
vector<tuple<string, string, string, vector<string>>> parseCircuit(const string circuitFile) {
    vector<tuple<string, string, string, vector<string>>> connections;
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
                // Parse input lines
                stringstream ss(line);
                string inputName;
                ss >> inputName;
                connections.emplace_back("", "", inputName, vector<string>());
            }
            else {
                // Parse component lines
                stringstream ss(line);
                string gateName, gateType, output, input;
                ss >> gateName >> gateType >> output;
                gateName.pop_back(); // Removing comma
                gateType.pop_back(); // Removing comma
                output.pop_back();   // Removing comma

                // Parse inputs for the component
                vector<string> inputs;
                while (ss >> input) {
                    if (input[input.length() - 1] == ',') {
                        input.pop_back(); // Removing comma
                    }
                    inputs.push_back(input);
                }

                // Create a tuple to store connection information and add it to the connections vector
                connections.emplace_back(gateName, gateType, output, inputs);
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
        char comma1, comma2;
        string input;
        int logic_value;
        ss >> time_stamp >> comma1 >> input >> logic_value;
        input.pop_back(); // Removing comma

        if (ss.fail() || comma1 != ',') {
            cerr << "Error parsing stimuli file: " << line << endl;
        }
        else {
            // Create a tuple to store stimuli information and add it to the stimuli vector
            stimuli.emplace_back(time_stamp, input, logic_value);
        }
    }

    file.close();
    return stimuli;
}

// Function to sort tuples by timestamp in ascending order
bool sortascen(const tuple<int, string, int>& a,
               const tuple<int, string, int>& b) {
    return (get<0>(a) < get<0>(b));
}

int main() {
    // Define file names for input and output
    string libraryFile = "library.lib";
    string circuitFile = "circuit.cir";
    string stimuliFile = "stimuli.stim";
    string outputFile = "simulation.sim"; // Output file name

    // Parse the library file to create gate objects
    vector<tuple<string, int, int>> Library_Gates = parseLibrary(libraryFile);

    // Parse the circuit file and determine gate connections
    vector<tuple<string, string, string, vector<string>>> connections = parseCircuit(circuitFile);

    // Initialize variables to store gate information, gate objects, and input values
    vector<Gate> GatesUsed;
    MultiInputGate* gatePtr = nullptr;
    unordered_map<string, int> inputs_values;

    // Iterate through connections to create gate objects and store inputs
    for (int j = 0; j < connections.size(); j++) {
        if (!(get<0>(connections[j]).empty())) {
            // Store input names and initialize their values to 0
            for (int k = 0; k < get<3>(connections[j]).size(); k++) {
                inputs_values[get<3>(connections[j])[k]] = 0;
            }
            inputs_values[get<2>(connections[j])] = 0;

            // Find corresponding gate type in the library and create gate object
            for (int i = 0; i < Library_Gates.size(); i++) {
                if (get<0>(Library_Gates[i]) == get<1>(connections[j])) {
                    gatePtr = nullptr;
                    int index_numOfInputs;
                    string GateType;
                    int numOfInputs;

                    // Determine gate type and number of inputs
                    if (get<0>(Library_Gates[i]) == "NOT") {
                        numOfInputs = 1;
                        GateType = "NOT";
                    }
                    else {
                        index_numOfInputs = get<0>(Library_Gates[i]).find_last_not_of("0123456789");
                        GateType = get<0>(Library_Gates[i]).substr(0, index_numOfInputs + 1);
                        numOfInputs = stoi(get<0>(Library_Gates[i]).substr(index_numOfInputs + 1));
                    }

                    // Create gate object based on gate type and number of inputs
                    if (get<2>(Library_Gates[i]) == numOfInputs) {
                        if (GateType == "NOT") {
                            gatePtr = new NOTGate();
                        }
                        else if (GateType == "AND") {
                            gatePtr = new ANDGate(numOfInputs);
                        }
                        else if (GateType == "OR") {
                            gatePtr = new ORGate(numOfInputs);
                        }
                        else if (GateType == "XOR") {
                            gatePtr = new XORGate(numOfInputs);
                        }
                        else if (GateType == "NAND") {
                            gatePtr = new NANDGate(numOfInputs);
                        }
                        else if (GateType == "NOR") {
                            gatePtr = new NORGate(numOfInputs);
                        }
                    }
                    else {
                        cout << "Invalid Number of Inputs for " << get<0>(Library_Gates[i]) << " " << get<2>(Library_Gates[i]) << endl;
                        exit(0);
                    }

                    // Store gate information and gate object
                    GatesUsed.emplace_back(get<1>(connections[j]), get<1>(Library_Gates[i]), gatePtr);
                    break;
                }
            }
        }
    }

    // Define the current time stamp
    int current_time = 0;

    // Evaluate gate outputs for initial time step and store output changes
    ofstream ofs;
    ofs.open(outputFile.c_str(), ofstream::out);
    vector<tuple<int, string, int>> ALLtimings;
    int count = 0;

    for (int i = 0; i < GatesUsed.size(); i++) {
        int index = i + connections.size() - GatesUsed.size();
        int Evaluation = GatesUsed[i].gatePtr->evaluate();
        if (Evaluation != inputs_values[get<2>(connections[index])]) {
            ALLtimings.emplace_back(current_time + GatesUsed[i].getDelay(), get<2>(connections[index]), Evaluation);
            inputs_values[get<2>(connections[index])] = Evaluation;
        }
    }

    // Parse the stimuli file and store stimuli data
    vector<tuple<int, string, int>> stimuli = parseStimuli(stimuliFile);
    for (const auto& stim : stimuli) {
        if (!(inputs_values[(get<1>(stim))] == get<2>(stim)))
            ALLtimings.emplace_back(get<0>(stim), get<1>(stim), get<2>(stim));
    }

    // Iterate through output changes, update gate inputs, and evaluate outputs
    while (count < ALLtimings.size()) {
        current_time = get<0>(ALLtimings[count]);
        inputs_values[get<1>(ALLtimings[count])] = get<2>(ALLtimings[count]);
        for (int i = 0; i < GatesUsed.size(); i++) {
            int index = i + connections.size() - GatesUsed.size();
            for (int z = 0; z < get<3>(connections[index]).size(); z++) {
                if ((get<3>(connections[index])[z] == get<1>(ALLtimings[count]))) {
                    GatesUsed[i].gatePtr->setInput(z, get<2>(ALLtimings[count]));
                    int Evaluation = GatesUsed[i].gatePtr->evaluate();
                    if (Evaluation != inputs_values[get<2>(connections[index])]) {
                        ALLtimings.emplace_back(current_time + GatesUsed[i].getDelay(), get<2>(connections[index]), Evaluation);
                        inputs_values[get<2>(connections[index])] = Evaluation;
                    }
                }
            }
        }
        count++;
    }

    // Sort output changes by timestamp in ascending order and write to output file
    sort(ALLtimings.begin(), ALLtimings.end(), sortascen);
    for (const auto& record : ALLtimings) {
        ofs << get<0>(record) << " " << get<1>(record) << " " << get<2>(record) << endl;
    }
    ofs.close();

    return 0;
}
