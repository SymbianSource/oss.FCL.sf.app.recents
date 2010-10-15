/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#ifndef LOGSPREDICTIVE12KEYTRANSLATOR_H
#define LOGSPREDICTIVE12KEYTRANSLATOR_H

#include <QObject>
#include <QChar>
#include <QHash>

#include "logspredictivetranslator.h"

const char StarKey = '*';
const char HashKey = '#';


/**
 * abstract translator for ITU-T 12 keys keyboard 
 *
 */
class LogsPredictive12KeyTranslator : public LogsPredictiveTranslator 
{

public: 

    ~LogsPredictive12KeyTranslator();
    
public: //from LogsPredictiveTranslator
    
    bool match( const QString& pattern, 
                LogsCntTokenIterator& names ) const;
    const QString translateChar( const QChar character ) const;

protected:
    
    explicit LogsPredictive12KeyTranslator( const HbInputLanguage& lang );

private:
    
    bool doSimpleMatch( const QString& pattern, LogsCntTokenIterator& names ) const;
    bool doComplexMatch( const QStringList& patternArray, LogsCntTokenIterator& names ) const;

private:
    
    friend class UT_LogsPredictive12KeyTranslator;
    
};

#endif //LOGSPREDICTIVE12KEYTRANSLATOR_H
