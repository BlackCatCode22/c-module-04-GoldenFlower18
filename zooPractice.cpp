#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

// Base Animal class
class Animal {
protected:
    std::string name;
    int age;
    std::string species;

public:
    // Constructor
    Animal(const std::string& name, int age, const std::string& species)
        : name(name), age(age), species(species) {}
    
    // Virtual destructor for proper inheritance
    virtual ~Animal() {}
    
    // Getters
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getSpecies() const { return species; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setSpecies(const std::string& newSpecies) { species = newSpecies; }
    
    // Virtual method to be overridden by subclasses
    virtual std::string makeSound() const {
        return "Generic animal sound";
    }
    
    // Virtual method for special characteristic
    virtual std::string getSpecialCharacteristic() const {
        return "No special characteristic";
    }
};

// Hyena subclass
class Hyena : public Animal {
private:
    bool isLaughing;

public:
    Hyena(const std::string& name, int age, bool isLaughing = true)
        : Animal(name, age, "Hyena"), isLaughing(isLaughing) {}
    
    std::string makeSound() const override {
        return "Hee-hee-hee!";
    }
    
    std::string getSpecialCharacteristic() const override {
        return isLaughing ? "Laughing hyena" : "Not currently laughing";
    }
    
    bool getIsLaughing() const { return isLaughing; }
    void setIsLaughing(bool laughing) { isLaughing = laughing; }
};

// Lion subclass
class Lion : public Animal {
private:
    bool isMale;
    float maneLength; // in inches, only applicable if male

public:
    Lion(const std::string& name, int age, bool isMale = false, float maneLength = 0.0)
        : Animal(name, age, "Lion"), isMale(isMale), maneLength(maneLength) {}
    
    std::string makeSound() const override {
        return "ROAR!";
    }
    
    std::string getSpecialCharacteristic() const override {
        if (isMale) {
            return "Male lion with " + std::to_string(maneLength) + " inch mane";
        } else {
            return "Female lion (huntress)";
        }
    }
    
    bool getIsMale() const { return isMale; }
    float getManeLength() const { return maneLength; }
    
    void setIsMale(bool male) { 
        isMale = male;
        if (!male) maneLength = 0.0; // Reset mane length if female
    }
    
    void setManeLength(float length) {
        if (isMale) {
            maneLength = length;
        }
    }
};

// Tiger subclass
class Tiger : public Animal {
private:
    int stripeCount;

public:
    Tiger(const std::string& name, int age, int stripeCount = 100)
        : Animal(name, age, "Tiger"), stripeCount(stripeCount) {}
    
    std::string makeSound() const override {
        return "Growl!";
    }
    
    std::string getSpecialCharacteristic() const override {
        return "Has approximately " + std::to_string(stripeCount) + " stripes";
    }
    
    int getStripeCount() const { return stripeCount; }
    void setStripeCount(int count) { stripeCount = count; }
};

// Bear subclass
class Bear : public Animal {
private:
    std::string bearType; // e.g. "Grizzly", "Polar", "Black"
    bool isHibernating;

public:
    Bear(const std::string& name, int age, const std::string& bearType = "Grizzly", bool isHibernating = false)
        : Animal(name, age, "Bear"), bearType(bearType), isHibernating(isHibernating) {}
    
    std::string makeSound() const override {
        return isHibernating ? "Zzzzz..." : "GROWL!";
    }
    
    std::string getSpecialCharacteristic() const override {
        return bearType + " bear" + (isHibernating ? " (hibernating)" : "");
    }
    
    std::string getBearType() const { return bearType; }
    bool getIsHibernating() const { return isHibernating; }
    
    void setBearType(const std::string& type) { bearType = type; }
    void setIsHibernating(bool hibernating) { isHibernating = hibernating; }
};

// Factory function to create the appropriate animal type
Animal* createAnimal(const std::string& species, const std::string& name, int age) {
    if (species == "Hyena") {
        return new Hyena(name, age);
    } else if (species == "Lion") {
        return new Lion(name, age);
    } else if (species == "Tiger") {
        return new Tiger(name, age);
    } else if (species == "Bear") {
        return new Bear(name, age);
    } else {
        // Default case for unknown species
        return new Animal(name, age, species);
    }
}

int main() {
    // Containers for animal data
    std::vector<Animal*> animals;
    std::map<std::string, int> speciesCount;
    
    // Read from input file
    std::ifstream inputFile("arrivingAnimals.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string name, species;
        int age;
        
        // Parse line format: Name,Age,Species
        if (std::getline(iss, name, ',') && 
            iss >> age && 
            iss.ignore() && // Skip the comma
            std::getline(iss, species)) {
            
            // Create appropriate animal type using factory function
            Animal* animal = createAnimal(species, name, age);
            animals.push_back(animal);
            
            // Update species count
            speciesCount[species]++;
        }
    }
    inputFile.close();
    
    // Write to output file
    std::ofstream outputFile("newAnimals.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl;
        // Clean up memory before exiting
        for (Animal* animal : animals) {
            delete animal;
        }
        return 1;
    }
    
    // Write report header
    outputFile << "Zoo Animal Intake Report" << std::endl;
    outputFile << "========================" << std::endl << std::endl;
    
    // Process each species type
    for (const auto& countPair : speciesCount) {
        const std::string& species = countPair.first;
        int count = countPair.second;
        
        outputFile << species << "s:" << std::endl;
        outputFile << "------" << std::endl;
        
        // List all animals of this species
        for (const Animal* animal : animals) {
            if (animal->getSpecies() == species) {
                outputFile << animal->getName() << ", " 
                         << animal->getAge() << " years old";
                
                // Add the special characteristic if available
                std::string characteristic = animal->getSpecialCharacteristic();
                if (characteristic != "No special characteristic") {
                    outputFile << " - " << characteristic;
                }
                
                outputFile << std::endl;
            }
        }
        
        // Add total count for this species
        outputFile << "Total " << species << "s: " << count << std::endl << std::endl;
    }
    
    // Add overall total
    outputFile << "Total animals: " << animals.size() << std::endl;
    
    outputFile.close();
    
    std::cout << "Animal processing complete. Report generated in newAnimals.txt" << std::endl;
    
    // Clean up memory
    for (Animal* animal : animals) {
        delete animal;
    }
    
    return 0;
}