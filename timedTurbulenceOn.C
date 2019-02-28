
#include "timedTurbulenceOn.H"
#include "Time.T.H"
#include "polyMesh.H"
#include "addToRunTimeSelectionTable.H"

#include <iostream>
#include <fstream>
#include <stdio.h>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(timedTurbulenceOn, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        timedTurbulenceOn,
        dictionary
    );
}
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


void Foam::functionObjects::timedTurbulenceOn::checkState()
{
        Info<< nl << type() << " functionObject checking state:" << endl;
        /*using namespace std;
        forAll(turbPropFiles_,i)
        {
            ifstream infile(turbPropFiles_[i], ifstream::in);
            ofstream outfile(turbPropFiles_[i]+Foam::name(pid()), ofstream::out);

            string findWord (" on");
            string newWord (" off");
            string line;

            while (getline(infile, line))
                    {
                    if (line.find(findWord) != string::npos) {

                    size_t found = line.find(findWord);

                    if (found!=string::npos)
                        line.replace(found,3,newWord);
                        outfile << line << std::endl;
                    }
                    else
                        outfile << line << std::endl;
                        continue;
                }

            infile.close();
            outfile.close();
                
            fileName origFile(turbPropFiles_[i]);
            fileName newFile(turbPropFiles_[i]+Foam::name(pid()));
            rm(origFile);

            mv(newFile, origFile);
        }*/
}
void Foam::functionObjects::timedTurbulenceOn::updateFile()
{
    //Info<< nl << type() << " functionObject checking state:" << endl;
    if (time_.value() >= turnOnTime_ )
    {
        
        Info<< nl << "The " << type() << " functionObject is turning turbulence on:" << endl;

        using namespace std;

        forAll(turbPropFiles_,i)
        {
            ifstream infile(turbPropFiles_[i], ifstream::in);
            ofstream outfile(turbPropFiles_[i]+Foam::name(pid()), ofstream::out);

            string findWord (" off");
            string newWord (" on");
            string line;

            while (getline(infile, line))
                    {
                    if (line.find(findWord) != string::npos) {

                    size_t found = line.find(findWord);

                    if (found!=string::npos)
                        line.replace(found,4,newWord);
                        outfile << line << std::endl;
                    }
                    else
                        outfile << line << std::endl;
                        continue;
                }

            infile.close();
            outfile.close();
                
            fileName origFile(turbPropFiles_[i]);
            fileName newFile(turbPropFiles_[i]+Foam::name(pid()));
            rm(origFile);

            mv(newFile, origFile);

        

        }
        flag = true;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::timedTurbulenceOn::timedTurbulenceOn
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    time_(runTime),
    turnOnTime_(),
    turbPropFiles_()
    //lastIndex_(-1)
{
    
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::timedTurbulenceOn::~timedTurbulenceOn()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::timedTurbulenceOn::read
(
    const dictionary& dict
)
{
    dict.lookup("turnOnTime") >> turnOnTime_;
    dict.lookup("turbPropFiles") >> turbPropFiles_;

    Info<< type() << "Implementing my third custom functionObject!!!!" << nl;
    Info<< type() << ": time vs file list:" << nl;
    forAll(turbPropFiles_, i)
    {
        turbPropFiles_[i] = turbPropFiles_[i].expand();
        if (!isFile(turbPropFiles_[i]))
        {
            FatalErrorInFunction
                << "File: " << turbPropFiles_[i] << " not found"
                << nl << exit(FatalError);
        }

        Info<< "    " << turbPropFiles_[i] << tab
            << turbPropFiles_[i] << endl;

    }
    Info<< endl;
    checkState();
    /*if (flag == false)
    {
        updateFile();
    }*/
    return true;
}


bool Foam::functionObjects::timedTurbulenceOn::execute()
{
    if (flag == false)
    {
        updateFile();
    }
    return true;
}


bool Foam::functionObjects::timedTurbulenceOn::write()
{
    return true;
}


// ************************************************************************* //