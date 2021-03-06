/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           |
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Contributors/Copyright:
    2016 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "swak.H"

#include "stateMachineSetStateFunctionObject.H"
#include "addToRunTimeSelectionTable.H"

#include "fvMesh.H"
#include "cellSet.H"
#include "faceSet.H"
#include "pointSet.H"
#include "swakTime.H"
#include "IOobjectList.T.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(stateMachineSetStateFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        stateMachineSetStateFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

stateMachineSetStateFunctionObject::stateMachineSetStateFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleFunctionObject(name,t,dict),
    machine_(
        StateMachine::machine(
            word(dict.lookup("machineName"))
        )
    ),
    state_(
        machine_.stateCode(
            word(dict.lookup("state"))
        )
    )
{
}

bool stateMachineSetStateFunctionObject::start()
{
    return true;
}

void stateMachineSetStateFunctionObject::writeSimple()
{
    Info << name() << " setting: "
        << machine_.force(state_) << endl;

    StateMachine::ensureWrite();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

} // namespace Foam


// ************************************************************************* //
