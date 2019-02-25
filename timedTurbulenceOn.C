
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

void Foam::functionObjects::timedTurbulenceOn::turnOn()
{
       //type() prints the type of the functionObject (i.e. timedTurbulenceOn)
        //timeVsFile_[i].second() prints the file name that will be copied
        //Info<< nl << type() << ": turning on turbulence in" << nl << timeVsFile_[i].second() << endl;

        /*fileName destFile(fileToUpdate_ + Foam::name(pid()));
        cp(timeVsFile_[i].second(), destFile);
        mv(destFile, fileToUpdate_);
        lastIndex_ = i;*/

}

void Foam::functionObjects::timedTurbulenceOn::updateFile()
{
    label i = lastIndex_;
    while
    (
        //checks if end of list has been reached
        //then checks if the trigger time (i.e. first()) is less than the
        //simulation time. If the two statements are true, nothing happens.
        //If the statement is false, increase i++ and run the next if statement.
        i < timeVsFile_.size()-1 && timeVsFile_[i+1].first() < time_.value()
    )
    {
        i++;
    }

    if (i > lastIndex_)
    {
        
        //type() prints the type of the functionObject (i.e. timedTurbulenceOn)
        //timeVsFile_[i].second() prints the file name that will be copied
        Info<< nl << type() << ": turning on turbulence in" << nl << timeVsFile_[i].second() << endl;

        /*
        fileName destFile(fileToUpdate_ + Foam::name(pid()));
        cp(timeVsFile_[i].second(), destFile);
        mv(destFile, fileToUpdate_);
        lastIndex_ = i;*/


        using namespace std;
        char orig[] = "C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties.water";
        char temp[] = "C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties_temp.water";
        ifstream infile("C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties.water", ifstream::in);
        ofstream outfile("C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties_temp.water", ofstream::out);

        char orig2[] = "C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties.air";
        char temp2[] = "C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties_temp.air";
        ifstream infile2("C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties.air", ifstream::in);
        ofstream outfile2("C:/Users/Jason/Desktop/bubbleColumn/constant/turbulenceProperties_temp.air", ofstream::out);

        string findWord (" off");
        string newWord (" on");
        string line;
        while (getline(infile, line))
                {
                    //cout <<  line <<'\n';
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
            remove(orig);

            int result=rename(temp,orig);

            string line2;
            while (getline(infile2, line2))
                {
                    //cout <<  line <<'\n';
                if (line2.find(findWord) != string::npos) {

                size_t found2 = line2.find(findWord);

                if (found2!=string::npos)
                    line2.replace(found2,4,newWord);
                    outfile2 << line2 << std::endl;
                }
                else
                    outfile2 << line2 << std::endl;
                    continue;
            }

            infile2.close();
            outfile2.close();
            remove(orig2);
            int result2=rename(temp2,orig2);
        lastIndex_ = i;
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
    fileToUpdate_(dict.lookup("fileToUpdate")),
    timeVsFile_(),
    lastIndex_(-1)
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
    dict.lookup("fileToUpdate") >> fileToUpdate_;
    dict.lookup("timeVsFile") >> timeVsFile_;

    lastIndex_ = -1;
    fileToUpdate_.expand();
    Info<< type() << "Implementing my third custom functionObject!!!!" << nl;
    Info<< type() << ": time vs file list:" << nl;
    forAll(timeVsFile_, i)
    {
        timeVsFile_[i].second() = timeVsFile_[i].second().expand();
        if (!isFile(timeVsFile_[i].second()))
        {
            FatalErrorInFunction
                << "File: " << timeVsFile_[i].second() << " not found"
                << nl << exit(FatalError);
        }

        Info<< "    " << timeVsFile_[i].first() << tab
            << timeVsFile_[i].second() << endl;
    }
    Info<< endl;

    updateFile();

    return true;
}


bool Foam::functionObjects::timedTurbulenceOn::execute()
{
    updateFile();

    return true;
}


bool Foam::functionObjects::timedTurbulenceOn::write()
{
    return true;
}


// ************************************************************************* //