/*
 *
 *  Copyright 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "Literal.h"
#include "Clause.h"
#include "util/VariableNames.h"

#include <cassert>

const Literal Literal::null( 0, POSITIVE );
const Literal Literal::conflict( 0, NEGATIVE );

ostream&
operator<<( 
    ostream& out, 
    const Literal& lit )
{
    if( !lit.isPositive() )
        out << "~";

    if( !VariableNames::isHidden( lit.getVariable() ) )
        out << VariableNames::getName( lit.getVariable() );
    #ifdef TRACE_ON
    else
        out << lit.getVariable();
    #endif
    
    return out;
}