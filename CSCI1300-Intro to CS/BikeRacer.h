#ifndef BIKERACER_H_INCLUDED
#define BIKERACER_H_INCLUDED

# include <iostream>
# include <fstream>

using namespace std;

class Racer
{
//Data Members
private:
    float MBike;
    float Cr;
    float MRider;
    float Vel;
    float K;
    float Draft;

//Access Methods
public:

    void setBikeMass(float m1);
    float getBikeMass();

    void setBikeCR(float res);
    float getBikeCR();

    void setRiderMass (float m2);
    float getRiderMass();

    void setCDraft(float inDraft);
    float getCDraft();

    void setK (float inK);
    float getK();

    void setVelocity (float V);
    float getVelocity();

    float Power();
    float TotalEnergy (float distance);

};


#endif // BIKERACER_H_INCLUDED
