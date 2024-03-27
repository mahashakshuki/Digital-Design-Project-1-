
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map> 

using namespace std;
// C++ program for the above approach 


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
    Gate(const string& n, const int& delay, MultiInputGate* gateptr) : name(n), delay(delay), gatePtr(gateptr) {}

    // Method to get gate's name
    string getName() const {
        return name;
    }

    // Method to get gate's name
    int getDelay() const {
        return delay;
    }
    // Virtual destructor

};

// NOT gate class
class NOTGate : public MultiInputGate {

public:
    // Constructor to initialize input
    NOTGate() : MultiInputGate(1){}

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
       
        for (int input : inputs) {
            if (input == 1) {
                return 1;
            }
        }
        return 0;
    }
};
//
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
        
        int countOnes = count(inputs.begin(), inputs.end(), 1);
       
        return countOnes % 2 == 1 ? 1 : 0;
    }
};

// NAND gate class
class NANDGate : public MultiInputGate {
public:
    // Constructor to initialize inputs vector with specified size
    NANDGate(size_t numInputs) : MultiInputGate(numInputs) {}

    // Function to evaluate XOR gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

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

    // Function to evaluate OR gate's output
    int evaluate() const override {
        // If there are no inputs, return 0
        if (inputs.empty()) {
            return 0;
        }

        for (int input : inputs) {
            if (input == 1) {
                return 0;
            }
        }
        return 1;
    }
};



// Function to parse the library file and create gate objects
// output tuple {Componenet_Name , delay, numofInputs}
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

        ss >> Component_Name >> numInputs >> comma >> expression >> delay;
        Component_Name.pop_back();// removing comma
        expression.pop_back();// removing comma
        // cout << Component_Name << " " << numInputs << " " << expression << " " << delay << endl;
        // Create gate objects based on gate type
        // MultiInputGate* gatePtr = nullptr;
        // if (Component_Name == "NOT") {
        //     gatePtr = new NOTGate();
        // }
        // else if (Component_Name == "AND2") {
        //     gatePtr = new ANDGate(numInputs);
        // }


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
                stringstream ss(line);
                string inputName;
                ss >> inputName;
                connections.emplace_back("", "", inputName, vector<string>());
            }
            else {
                stringstream ss(line);
                string gateName, gateType, output, input;
                ss >> gateName >> gateType >> output;
                gateName.pop_back();// removing comma
                gateType.pop_back();// removing comma
                output.pop_back();// removing comma                
                // cout << gateName << " " << gateType << " " << output << " ";
                vector<string> inputs;
                while (ss >> input) {
                    if (input[input.length() - 1] == ',')
                        input.pop_back();// removing comma 
                    // cout<< input<< " ";
                    inputs.push_back(input);
                }
                // cout<<endl;

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
        char   comma1, comma2;
        string input;
        int logic_value;
        ss >> time_stamp >> comma1 >> input >> logic_value;
        input.pop_back();
        // cout << time_stamp << " " << input << " " << logic_value<<endl;
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


int main() {
    string libraryFile = "library.lib";
    string circuitFile = "circuit.cir";
    string stimuliFile = "stimuli.stim";
    string outputFile = "simulation.sim"; // Output file name

    // Open the output file for writing
    // ofstream outputFileStream(outputFile);
    // if (!outputFileStream.is_open()) {
    //     cerr << "Error: Unable to open output file '" << outputFile << "'." << endl;
    //     return EXIT_FAILURE;
    // }

    // Parse the library file to create gate objects
    vector<tuple<string, int,int>>  Library_Gates = parseLibrary(libraryFile);
    // Parse the circuit file and determine gate connections


    vector<tuple<string, string, string, vector<string>>> connections = parseCircuit(circuitFile);

    vector<Gate> GatesUsed;
    MultiInputGate* gatePtr = nullptr;
    unordered_map<string, int> inputs_values;
    for (int j = 0; j < connections.size(); j++)
    {
        if (!(get<0>(connections[j]).empty()))
        {

            for (int k = 0; k < get<3>(connections[j]).size(); k++)
                inputs_values[get<3>(connections[j])[k]] = 0;
            inputs_values[get<2>(connections[j])] = 0;
            for (int i = 0; i < Library_Gates.size(); i++)
                if (get<0>(Library_Gates[i]) == get<1>(connections[j]))
                {
                    gatePtr = nullptr;
                    int index_numOfInputs; 
                    string GateType;

                    int numOfInputs;
                    if (get<0>(Library_Gates[i]) == "NOT")
                    {
                        numOfInputs = 1;
                        GateType = "NOT";
                    }
                    else
                    {
                        index_numOfInputs = get<0>(Library_Gates[i]).find_last_not_of("0123456789");
                        GateType = get<0>(Library_Gates[i]).substr(0, index_numOfInputs + 1);

                        numOfInputs = stoi(get<0>(Library_Gates[i]).substr(index_numOfInputs + 1));
                    }

                    if (get<2>(Library_Gates[i]) == numOfInputs)
                    {
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
                    else
                    {
                        cout << "Invalid Number of Inputs for " << get<0>(Library_Gates[i]) << " " << get<2>(Library_Gates[i]) << endl;
                        exit(0);
                    }
                   
                    // cout<< get<1>(Library_Gates[i])<< " " << get<0>(Library_Gates[i]) << " " << gatePtr <<endl;
                    GatesUsed.emplace_back(get<1>(connections[j]), get<1>(Library_Gates[i]), gatePtr);
                    break;
                }

        }

    }


    // Define the current time stamp

    int current_time = 0;
    // Evaluate gate outputs for initial time step
    ofstream ofs;
    ofs.open(outputFile.c_str(), ofstream::out);

    for (int i = 0; i < GatesUsed.size(); i++)
    {
        int index = i + connections.size() - GatesUsed.size();
        int Evaluation = GatesUsed[i].gatePtr->evaluate();
        if (Evaluation != inputs_values[get<2>(connections[index])])
        {
            ofs << current_time+ GatesUsed[i].getDelay() << ',' << " " << get<2>(connections[index]) << ',' << " " << Evaluation << endl;
            inputs_values[get<2>(connections[index])] = Evaluation;
            current_time = current_time + GatesUsed[i].getDelay();

        }
    }

    // // Parse the stimuli file and store stimuli data
    vector<tuple<int, string, int>> stimuli = parseStimuli(stimuliFile);
    // // Redirect output stream to the output file
    // streambuf *coutbuf = cout.rdbuf();
    // // cout.rdbuf(outputFileStream.rdbuf());
    for (const auto& stim : stimuli)
    {

        if (!(inputs_values[get<1>(stim)] == get<2>(stim)))
        {
            ofs << get<0>(stim) << ',' << " " << get<1>(stim) << ',' << " " << get<2>(stim) << endl;
            inputs_values[get<1>(stim)] = get<2>(stim);
            current_time = get<0>(stim);
            for (int i = 0; i < GatesUsed.size(); i++)
            {
                int index = i + connections.size() - GatesUsed.size();
                for (int z = 0; z < get<3>(connections[index]).size();z++) {
                    if ((get<3>(connections[index])[z] == get<1>(stim)))
                    {
                        GatesUsed[i].gatePtr->setInput(z, get<2>(stim));
                        //cout << get<3>(connections[index])[z] << " " << GatesUsed[i].gatePtr->inputs[z] << endl;
                        int Evaluation = GatesUsed[i].gatePtr->evaluate();
                        //cout << get<2>(connections[index]) << " " << Evaluation << endl;

                        if (Evaluation != inputs_values[get<2>(connections[index])])
                        {
                            
                            ofs << current_time + GatesUsed[i].getDelay() << ',' << " " << get<2>(connections[index]) << ',' << " " << Evaluation << endl;
                            inputs_values[get<2>(connections[index])] = Evaluation;
                            current_time = current_time + GatesUsed[i].getDelay();
                        }
                    }
                }


            }
        }
    }
    ofs.close();
    
    exit(0);
    return 0;
    //         

}
