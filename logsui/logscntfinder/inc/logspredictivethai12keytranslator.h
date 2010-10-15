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

#ifndef LOGSPREDICTIVETHAI12KEYTRANSLATOR_H
#define LOGSPREDICTIVETHAI12KEYTRANSLATOR_H

#include "logspredictive12keytranslator.h"


//Name: TIS-620
//MIBenum: 2259
//Source: Thai Industrial Standards Institute (TISI) [Tantsetthi]
const int MIBenumThai = 2259;



/**
 * translator for Thai ITU-T 12 keys keyboard 
 *
 */
class LogsPredictiveThai12KeyTranslator : public LogsPredictive12KeyTranslator 
{

public: 

    explicit LogsPredictiveThai12KeyTranslator();
    explicit LogsPredictiveThai12KeyTranslator( const HbInputLanguage& lang );
    ~LogsPredictiveThai12KeyTranslator();
    
public: //from LogsPredictiveTranslator
    
    QStringList patternTokens( const QString& pattern ) const;
    int hasPatternSeparators( const QString& pattern ) const;
    QString& trimPattern( QString& pattern, bool tailOnly ) const;
    QStringList nameTokens( const QString& name ) const;
    const QString translateChar( const QChar character, bool& ok ) const;
    inline int mib() const {return MIBenumThai;}
  
private:
    
    bool isIgnored( const QChar character ) const;
    
private:
    
    friend class UT_LogsPredictiveThai12KeyTranslator;
    
};

#endif //LOGSPREDICTIVETHAI12KEYTRANSLATOR_H
