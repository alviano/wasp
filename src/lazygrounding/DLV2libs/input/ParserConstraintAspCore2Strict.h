/* 
 * File:   ParserConstraintAspCore2Strict.h
 * Author: cesco
 *
 * Created on 3 giugno 2014, 18.40
 */

#ifndef PARSERCONSTRAINTASPCORE2STRICT_H
#define	PARSERCONSTRAINTASPCORE2STRICT_H

#include "ParserConstraint.h"

namespace DLV2
{

    class ParserConstraintAspCore2Strict : public ParserConstraint{
    public:
        ParserConstraintAspCore2Strict() {}
        virtual ~ParserConstraintAspCore2Strict() {}
        
        inline virtual void rangeFacts();
        inline virtual void directives();
    };
};

void
DLV2::ParserConstraintAspCore2Strict::rangeFacts()
{
    DLV2::ErrorMessage::errorDuringParsing("range facts are not in the ASPCore2 standard!");
}

void
DLV2::ParserConstraintAspCore2Strict::directives()
{
    DLV2::ErrorMessage::errorDuringParsing("directives are not in the ASPCore2 standard!");
}

#endif	/* PARSERCONSTRAINTASPCORE2STRICT_H */

