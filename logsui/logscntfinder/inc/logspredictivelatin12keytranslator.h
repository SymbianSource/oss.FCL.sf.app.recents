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

#ifndef LOGSPREDICTIVELATIN12KEYTRANSLATOR_H
#define LOGSPREDICTIVELATIN12KEYTRANSLATOR_H

#include <QObject>
#include <QChar>
#include <QHash>

#include "logspredictive12keytranslator.h"


/**
 * translator for Latin ITU-T 12 keys keyboard 
 *
 */
class LogsPredictiveLatin12KeyTranslator : public LogsPredictive12KeyTranslator 
{

public: 

    explicit LogsPredictiveLatin12KeyTranslator();
    ~LogsPredictiveLatin12KeyTranslator();

public: //from LogsPredictiveTranslator
    
    QStringList nameTokens( const QString& name ) const;
    
public: //from LogsPredictive12KeyTranslator
    
    const QChar translateChar( const QChar character ) const;
    

private:
    
    QHash<QChar,QChar> mSpecialKeyMap;
    friend class UT_LogsPredictiveLatin12KeyTranslator;
    
};

#endif //LOGSPREDICTIVELATIN12KEYTRANSLATOR_H
