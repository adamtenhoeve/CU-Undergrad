# include <iostream>
# include <fstream>

# include "BikeRacer.h"

using namespace std;

void Racer::setBikeMass (float m1)
{
    MBike = m1;
}

float Racer::getBikeMass()
{
    return MBike;
}

void Racer::setBikeCR (float res)
{
    Cr = res;
}

float Racer::getBikeCR ()
{
    return Cr;
}

void Racer::setCDraft (float inDraft)
{
    Draft = inDraft;
}

void Racer::setRiderMass (float m2)
{
    MRider = m2;
}

float Racer::getRiderMass()
{
    return MRider;
}

float Racer::getCDraft ()
{
    return Draft;
}

void Racer::setK (float inK)
{
    K = inK;
}

float Racer::getK ()
{
    return K;
}

void Racer::setVelocity (float V)
{
    Vel = V;
}

float Racer::getVelocity ()
{
    return Vel;
}

float Racer::Power ()
{
    float Pair = K * Draft * Vel * Vel * Vel;
    float Proll = Cr * 9.8 * (MBike + MRider) * Vel;
    float Psec = Pair + Proll;
    return Psec;
}

float Racer::TotalEnergy (float distance)
{
    float Pow = Power ();
    float time = distance * 1000 / Vel;
    float Energy = Pow * time;
    return Energy;
}
