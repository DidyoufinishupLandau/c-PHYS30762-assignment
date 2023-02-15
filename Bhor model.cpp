//Author Pan Zhang
//Given any input, calculate photon energy emitted from hydrogen-like atom
#include<iostream>
#include<cmath>
#include <iomanip>
#include <sstream>

double photon_energy(int atomic_number, int initial_energy_level, int final_energy_level, std::string units) {
    double energy;

    energy = 13.6 * pow(atomic_number, 2) * (pow(final_energy_level, -2) - pow(initial_energy_level, -2));

    if (units == "J") {
        return energy * 1.602 * pow(10, -16);
    }
    else {
        return energy;
    }
}

bool if_integer(std::string input) {
    for (int i = 0; i < input.length(); i++) {
        if (!isdigit(input[i])) {
            return false;
        }
    }
    return true;
}
void input(int* entry) {
    std::string temp;
    std::cin >> temp;
    //here I tried to avoid the issue of cin.fail when it encounter 8-, or any 'int-' as c++ identify - as int.
    //The if_integer also avoids negative value input. 
    while (!if_integer(temp)|| std::stoi(temp)==0) {
        std::cout << "Only enter a positive integer number(with no signs). Please re-enter: " << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> temp;
    }
    *entry = std::stoi(temp);
}

void input_string(std::string *entry, const std::string decision_1, const std::string decision_2) {
    std::cin >> *entry;
    while ((*entry != decision_1) && (*entry != decision_2)) {
        std::cout << "Please enter"<< " "<< decision_1 << " "<<"or"<< " " << decision_2 << std::endl;
        std::cin.clear();
        std::cin.ignore();
        std::cin >> *entry;
    }
}

int main() {
    std::string units;
    std::string continue_or_not{"Y"};

    const std::string YES{"Y"};
    const std::string NO{ "N" };
    const std::string JOULES = "J";
    const std::string ELECTRON_VOLTS = "eV";

    int atomic_number;
    int initial_energy_level;
    int final_energy_level;

    while (continue_or_not == "Y") {

        std::cout << "Please enter atomic number: " << std::endl;
        input(&atomic_number);
        std::cout << "Please enter initial energy level: " << std::endl;
        input(&initial_energy_level);
        std::cout << "Please enter final energy level: " << std::endl;
        input(&final_energy_level);

        while (initial_energy_level < final_energy_level) {
            std::cout << "Impossible for a photon to"
                "gain energy if the initial energy level is" 
                "smaller than the final energy level. " << std::endl;

            std::cout << "Please enter initial energy level: " << std::endl;
            input(&initial_energy_level);

            std::cout << "Please enter final energy level: " << std::endl;
            input(&final_energy_level);
        }

        std::cout << "Calculate in joules or electron volts? [J/eV]" << std::endl;
        input_string(&units, JOULES, ELECTRON_VOLTS);

        std::cout << "The energy of the emitted photon is: " << std::setprecision(3) << photon_energy(atomic_number, initial_energy_level, final_energy_level, units) << " " << units << std::endl;

        std::cout << "Repeat calculation?[N(No)/Y(Yes)]" << std::endl;
        input_string(&continue_or_not, YES, NO);
    }
    std::cout << "######################################" << std::endl;
    std::cout << "program complete!" << std::endl;
    return 0;
}