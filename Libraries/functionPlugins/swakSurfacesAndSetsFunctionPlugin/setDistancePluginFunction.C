/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
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
    2012-2013, 2016-2017 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "setDistancePluginFunction.H"
#include "FieldValueExpressionDriver.H"

#include "addToRunTimeSelectionTable.H"

namespace Foam {

defineTypeNameAndDebug(setDistancePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction, setDistancePluginFunction , name, setDistance);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

setDistancePluginFunction::setDistancePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    GeneralSetsPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        string("setName primitive word")
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void setDistancePluginFunction::doEvaluation()
{
    autoPtr<volScalarField> pDistance(
        new volScalarField(
            IOobject(
                "setDistanceInCell",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            dimensionedScalar("no",dimless,pTraits<scalar>::max)
        )
    );

    const vectorField &cc=mesh().C();

    List<vectorField> faceCenters(Pstream::nProcs());
    faceCenters[Pstream::myProcNo()]=theSet();
    Pstream::scatterList(faceCenters);

    forAll(cc,cellI) {
        const vector &here=cc[cellI];
        scalar &minDist=(pDistance)()[cellI];

        forAll(faceCenters,i){
            const vectorField &fc=faceCenters[i];
            forAll(fc,faceI){
                const scalar dist=mag(here-fc[faceI]);
                if(dist<minDist) {
                    minDist=dist;
                }
            }
        }
    }

    pDistance->correctBoundaryConditions();

    result().setObjectResult(pDistance);
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

} // namespace

// ************************************************************************* //
