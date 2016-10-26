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
#include "ExtendedStatistics.h"
#include "../Solver.h"
#include "VariableNames.h"

#ifdef ESTATS_ON

ExtendedStatistics* ExtendedStatistics::stats = NULL;

void ExtendedStatistics::printExtendedStatistics()
{
    cerr << separator << endl;
    cerr << "Extended Statistics" << endl << endl;
    
    cerr << "Atom Name\t#conflictual\t#used for learning\t#added in learned clause\t#selected as choice" << endl;
    for( unsigned int i = 1; i < varStats.size(); i++ )
        if( !VariableNames::isHidden( i ) )
        {
            unsigned int sum = varStats[ i ]->conflictLiteral + varStats[ i ]->usedInConflictResolution + varStats[ i ]->inLearnedClause + varStats[ i ]->usedAsChoice;
            if( sum > 0 )
                cerr << VariableNames::getName( i ) << "\t" << varStats[ i ]->conflictLiteral << "\t" << varStats[ i ]->usedInConflictResolution << "\t" << varStats[ i ]->inLearnedClause << "\t" << varStats[ i ]->usedAsChoice << endl;
        }
    cerr << separator << endl;    
}
#endif