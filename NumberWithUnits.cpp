#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;

#include "NumberWithUnits.hpp"

using namespace ariel;

const double epsilon = 0.001;

std::unordered_map<std::string, std::unordered_map<std::string, double>> convertion;


    void NumberWithUnits::searchFor(string leftKey, string rightKey, double amount){
        convertion[leftKey][rightKey] = amount;
        if(!(convertion.count(rightKey)!=0 && convertion[rightKey].count(leftKey) != 0)){
            searchFor(rightKey, leftKey, 1/amount);
        }

        for (auto& iter: convertion[rightKey]) {
            if(!(convertion.count(leftKey)!=0 && convertion[leftKey].count(iter.first) != 0) && leftKey != iter.first){
                searchFor(leftKey, iter.first, iter.second*amount); 
            }
        }
    }   


    void NumberWithUnits::read_units(ifstream& myfile){
        double leftAmount = 0;
        double rightAmount = 0;
        string left;
        string right;
        string equalTo;
        bool first = true;
        if (myfile.is_open()){
            while(myfile >> leftAmount >> left >> equalTo >> rightAmount >> right){
                if(leftAmount == 1){
                    searchFor(left, right, rightAmount);
                }    
            }    
        }

        myfile.close();

    }     
    

    NumberWithUnits::NumberWithUnits(const double& givenamount, const string& mesurment){// Constructor
        this->amount = givenamount;
        
        if(convertion.count(mesurment)!=0){
            this->measure = mesurment;
        }

        else{
            throw std::invalid_argument("The mesurment [" + mesurment + "] is not exist");
            
        }
        
    }


    ostream& ariel::operator<< (ostream& os, const NumberWithUnits& myself){
        os << myself.amount << "["<< myself.measure << "]";
        return os;
    }

    istringstream& ariel::operator>> (istringstream& is, NumberWithUnits& myself){
        double amo = 0;
        string mea;
        char open;
        char close;
    
        if(is >> skipws >> amo >> open >> mea){
            if (mea.at(mea.length() - 1) == ']'){
                mea = mea.substr(0, mea.length() - 1);
            }

            else{
                is >> skipws >> close;
            }
        }

        if(convertion.count(mea) == 0){
            throw std::invalid_argument("The mesurment [" + mea + "] is not exist");
        }
        
        myself.amount = amo;
        myself.measure = mea;

        return is;
    }

    NumberWithUnits ariel::operator+(const NumberWithUnits& myself){ // Return out itself
        return myself;
    }

    bool ariel::searchSameMessure(const string& myMeasure, const string& otherMesure){ // Check if we can convert the two measuremnts 
        if(!(convertion.count(otherMesure)!=0 && convertion[otherMesure].count(myMeasure) != 0)){
            throw invalid_argument("Units do not match - [" + myMeasure + "] cannot be converted to [" + otherMesure + "]");
        }

        else{
            return true;
        }

    }


   

     NumberWithUnits& ariel::NumberWithUnits::operator+=(const NumberWithUnits& other){
         if(other.measure == this->measure){
            this->amount += other.amount; 
        }

        else{
            bool sameMeasure = searchSameMessure(this->measure, other.measure);

            if(sameMeasure){
                this->amount += convertion[other.measure][this->measure]*other.amount;

            }
        }

        return *this;
     }

    NumberWithUnits ariel::operator+ (const NumberWithUnits& myself, const NumberWithUnits& other){
        double tmp = 0;
        bool same = false;
        if(myself.measure == other.measure){
            same = true;
        }


        else{
            bool sameMeasure = searchSameMessure(myself.measure, other.measure);

            if(sameMeasure){
                tmp = convertion[other.measure][myself.measure]*other.amount;
            }
        }
    
        if(same){
            return NumberWithUnits(myself.amount + other.amount, myself.measure);
        }
        
        return NumberWithUnits(myself.amount+tmp, myself.measure); 
    }

    NumberWithUnits ariel::operator-(const NumberWithUnits& myself){

        return NumberWithUnits(-myself.amount, myself.measure); 
    }


    NumberWithUnits& ariel::NumberWithUnits::operator-=(const NumberWithUnits& other){
         if(other.measure == this->measure){
            this->amount -= other.amount; 
        }

        else{
            bool sameMeasure = searchSameMessure(this->measure, other.measure);

            if(sameMeasure){
                this->amount -= convertion[other.measure][this->measure]*other.amount;

            }
        }

        return *this;
     }
    

    NumberWithUnits ariel::operator - (const NumberWithUnits& myself, const NumberWithUnits& other){
        double tmp = 0;
        bool same = false;
        if(myself.measure == other.measure){
            same = true;
        }


        else{
            bool sameMeasure = searchSameMessure(myself.measure, other.measure);

            if(sameMeasure){
                tmp = convertion[other.measure][myself.measure]*other.amount;
            }
        }
    
        if(same){
            return NumberWithUnits(myself.amount - other.amount, myself.measure);
        }
        
        return NumberWithUnits(myself.amount-tmp, myself.measure); 
    }


    bool ariel::operator == (const NumberWithUnits& myself, const NumberWithUnits& other){
        if(other.measure == myself.measure){
            return other.amount == myself.amount || abs(myself.amount - other.amount) < epsilon;
        }

        else{
        
            bool sameMeasure = searchSameMessure(myself.measure, other.measure);
            if(sameMeasure){
                double checking = convertion[other.measure][myself.measure]*other.amount;


                return (myself.amount == checking || abs(myself.amount - checking) < epsilon);
                
            }
        }

        return false;
    }
            
    bool ariel::operator != (const NumberWithUnits& myself, const NumberWithUnits& other){
        return (!(myself==other));
    }

    bool ariel::operator >= (const NumberWithUnits& myself, const NumberWithUnits& other){
        return (myself>other || myself==other);
    }
            
    bool ariel::operator <= (const NumberWithUnits& myself, const NumberWithUnits& other){
        return (myself<other || myself==other);

    }

    bool ariel::operator > (const NumberWithUnits& myself, const NumberWithUnits& other){

        if(other.measure == myself.measure){
            return myself.amount > other.amount;
        }

        else{
            pair<string, double> mine(myself.measure, myself.amount);
            pair<string, double> his(other.measure, other.amount);
            bool sameMeasure = searchSameMessure(mine.first, his.first);

            if(sameMeasure){
                double checking = convertion[other.measure][myself.measure]*other.amount;

                if(myself.amount > checking){
                    return true;
                }

                else{
                    return false; 
                }
            }
        }

        return false;
    }
            
    bool ariel::operator < (const NumberWithUnits& myself, const NumberWithUnits& other){
        return (!(myself>other) && myself != other);
    }

    NumberWithUnits& ariel::operator ++(NumberWithUnits& myself){
        myself.amount++;
        return myself;
    }

    NumberWithUnits ariel::operator ++(NumberWithUnits& myself, int dummy){
        string strTmp = myself.measure;
        double tmp = myself.amount;
        myself.amount++;
        return NumberWithUnits{tmp, strTmp};
    }

    NumberWithUnits& ariel::operator --(NumberWithUnits& myself){
        myself.amount--;
        return myself;
    }

    NumberWithUnits ariel::operator --(NumberWithUnits& myself ,int dummy){
        string strTmp = myself.measure;
        double tmp = myself.amount;
        myself.amount--;
        return NumberWithUnits{tmp, strTmp};
        
    }

    NumberWithUnits ariel::operator* (const double& mutiplyer, const NumberWithUnits& myself){
        return NumberWithUnits(myself.amount*mutiplyer, myself.measure);
    }

    NumberWithUnits ariel::operator* (const NumberWithUnits& myself, const double& mutiplyer){
        return NumberWithUnits(myself.amount*mutiplyer, myself.measure);
    }

     NumberWithUnits ariel::operator* (const int& mutiplyer, const NumberWithUnits& myself){
        double tmp = (double) mutiplyer;
        return NumberWithUnits(myself.amount*mutiplyer, myself.measure);
    }

    NumberWithUnits ariel::operator* (const NumberWithUnits& myself, const int& mutiplyer){
        double tmp = (double) mutiplyer;
        return NumberWithUnits(myself.amount*tmp, myself.measure);
    }


    void ariel::NumberWithUnits::setAmount(double newAmount){
        this->amount = newAmount;
    }

    void ariel::NumberWithUnits::setMeasure(std::string newMeasure){
        this->measure = newMeasure;
    }

    double ariel::getAmount(const NumberWithUnits myself){
        return myself.amount;
    }

    std::string ariel::getMeasure(const NumberWithUnits myself){
        return myself.measure;
    }
