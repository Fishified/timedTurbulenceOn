/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::functionObjects::timedTurbulenceOn

Group
    grpUtilitiesFunctionObjects

Description
    Performs a file copy/replacement once a specified time has been reached.

    Example usage to update the fvSolution dictionary at various times
    throughout the calculation:

    \verbatim
    fileUpdate1
    {
        type              timedTurbulenceOn;
        libs              ("libutilityFunctionObjects.so");
        writeControl      timeStep;
        writeInterval     1;
        fileToUpdate      "$FOAM_CASE/system/fvSolution";
        timeVsFile
        (
            (-1 "$FOAM_CASE/system/fvSolution.0")
            (0.10 "$FOAM_CASE/system/fvSolution.10")
            (0.20 "$FOAM_CASE/system/fvSolution.20")
            (0.35 "$FOAM_CASE/system/fvSolution.35")
        );
    }
    \endverbatim

SourceFiles
    timedTurbulenceOn.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_timedTurbulenceOn_H
#define functionObjects_timedTurbulenceOn_H

#include "functionObject.H"
#include "Tuple2.T.H"
#include <iostream>
#include <fstream>
#include <stdio.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Time;

namespace functionObjects //nested namespaces
{

/*---------------------------------------------------------------------------*\
                  Class timedTurbulenceOn Declaration
\*---------------------------------------------------------------------------*/

/*Things left to do:

1. Make it check whether turbulence is on already if the case is starting from 0
   -and have it change turn it off if it is already on
2. Test on a parallel case
3. Make variable names meaningful

*/

class timedTurbulenceOn
:
    public functionObject //inherit from Foam::functionObject class
{
    // Private data

        //- Reference to Time, Time is a Foam::Time instance?
        const Time& time_;

        //- Name of file to update
        scalar turnOnTime_; //fileToUpdate is now an instance of the Foam::fileName class
        bool flag = false;
        //- List of times vs filenames
        //List<Tuple2<scalar, fileName>> turbPropFiles_; //timeVsFile is now an instance of Foam::List class
        List<fileName> turbPropFiles_;
        //- Index of last file copied
        //label lastIndex_; //lastIndex_ is now a int type variable for use in OpenFoam


    // Private Member Functions

        //- Update file
        void updateFile();
        void checkState();

        //- Disallow default bitwise copy construct
        timedTurbulenceOn(const timedTurbulenceOn&);

        //- Disallow default bitwise assignment
        void operator=(const timedTurbulenceOn&);


public:

    //- Runtime type information
    TypeName("timedTurbulenceOn");


    // Constructors

        //- Construct from Time and dictionary
        timedTurbulenceOn
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

    //- Destructor
    virtual ~timedTurbulenceOn();


    // Member Functions

        //- Read the timedTurbulenceOn data
        virtual bool read(const dictionary&);

        //- Execute file updates
        virtual bool execute();

        //- Do nothing
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //