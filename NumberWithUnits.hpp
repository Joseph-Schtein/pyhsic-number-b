#ifndef HEADER_HPP
#define HEADER_HPP

#include <utility>
#include <string>
#include <iostream>
#include <unordered_map>

namespace ariel{
    class NumberWithUnits{
        public:
            double amount;
            std::string measure;

        static void searchFor(std::string leftKey, std::string rightKey, double amount);    
        
        NumberWithUnits(const double& givenamount, const std::string& mesurment);

        static void read_units(std::ifstream &file);

        void setAmount(double newAmount);

        void setMeasure(std::string newMeasure);

        friend double getAmount(const NumberWithUnits myself);

        friend std::string getMeasure(const NumberWithUnits myself);

        friend std::ostream& operator << (std::ostream& os, const NumberWithUnits& myself);

        friend std::istringstream& operator >> (std::istringstream& is, NumberWithUnits& myself);     

        friend NumberWithUnits operator+ (const NumberWithUnits& myself);

        NumberWithUnits& operator+=(const NumberWithUnits& other);

        friend NumberWithUnits operator+(const NumberWithUnits& myself, const NumberWithUnits& other);

        friend NumberWithUnits operator-(const NumberWithUnits& myself);

        NumberWithUnits& operator -= (const NumberWithUnits& other);

        friend NumberWithUnits operator - (const NumberWithUnits& myself, const NumberWithUnits& other);

        friend bool operator == (const NumberWithUnits& myself, const NumberWithUnits& other);
            
        friend bool operator != (const NumberWithUnits& myself, const NumberWithUnits& other);

        friend bool operator >= (const NumberWithUnits& myself, const NumberWithUnits& other);
            
        friend bool operator <= (const NumberWithUnits& myself, const NumberWithUnits& other);

        friend bool operator > (const NumberWithUnits& myself, const NumberWithUnits& other);
            
        friend bool operator < (const NumberWithUnits& myself, const NumberWithUnits& other);
        
        friend NumberWithUnits& operator ++(NumberWithUnits& myself);

        friend NumberWithUnits operator ++(NumberWithUnits& myself, int dummy);

        friend NumberWithUnits& operator --(NumberWithUnits& myself);

        friend NumberWithUnits operator --( NumberWithUnits& myself, int dummy);

        friend NumberWithUnits operator* (const double& mutiplyer, const NumberWithUnits& myself);

        friend NumberWithUnits operator* (const NumberWithUnits& myself, const double& mutiplyer);

        friend NumberWithUnits operator* (const int& mutiplyer, const NumberWithUnits& myself);

        friend NumberWithUnits operator* (const NumberWithUnits& myself, const int& mutiplyer);

    };

    bool searchSameMessure(const std::string& myMeasure, const std::string&  otherMesure);

}


#endif