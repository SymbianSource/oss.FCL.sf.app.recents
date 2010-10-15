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

#include "logspredictive12keytranslator.h"
#include <QHash>


//Name: ISO_8859-1:1987 [RFC1345,KXS2]
//MIBenum: 4
//Source: ECMA registry
const int MIBenumLatin = 4;

/**
 * translator for Latin ITU-T 12 keys keyboard 
 *
 */
class LogsPredictiveLatin12KeyTranslator : public LogsPredictive12KeyTranslator 
{

public: 

    explicit LogsPredictiveLatin12KeyTranslator();
    explicit LogsPredictiveLatin12KeyTranslator( const HbInputLanguage& lang );
    ~LogsPredictiveLatin12KeyTranslator();

public: //from LogsPredictiveTranslator
    
    QStringList patternTokens( const QString& pattern ) const;
    int hasPatternSeparators( const QString& pattern ) const;
    QString& trimPattern( QString& pattern, bool tailOnly ) const;
    QStringList nameTokens( const QString& name ) const;
    inline int mib() const {return MIBenumLatin;}
    const QString translateChar( const QChar character, bool& ok ) const;
    
private:
    
    void createSpecialMapping();
    void splitPattern( const QString& pattern, QString& car, QString& cdr ) const;

private:
    
    QHash<QChar,QChar> mSpecialKeyMap;
    friend class UT_LogsPredictiveLatin12KeyTranslator;
    
};

#endif //LOGSPREDICTIVELATIN12KEYTRANSLATOR_H
