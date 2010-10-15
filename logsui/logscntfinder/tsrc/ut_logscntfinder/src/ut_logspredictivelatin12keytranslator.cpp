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
#include "ut_logspredictivelatin12keytranslator.h"
#include "logspredictivelatin12keytranslator.h"

#include <hbinputsettingproxy.h>
#include <QtTest/QtTest>
#include <QSignalSpy>


void UT_LogsPredictiveLatin12KeyTranslator::initTestCase()
{
}

void UT_LogsPredictiveLatin12KeyTranslator::cleanupTestCase()
{
}


void UT_LogsPredictiveLatin12KeyTranslator::init()
{
    mTranslator = 
       new LogsPredictiveLatin12KeyTranslator( 
               LogsPredictiveTranslator::currentLanguage() );
}

void UT_LogsPredictiveLatin12KeyTranslator::cleanup()
{
    delete mTranslator;
    mTranslator = 0;
}

void UT_LogsPredictiveLatin12KeyTranslator::testConstructor()
{
    QVERIFY( mTranslator );
    QVERIFY( !mTranslator->mSpecialKeyMap.isEmpty() );
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testTranslateText()
{
    QCOMPARE( mTranslator->translateText( QString( "Nokia" ) ), QString( "66542" ) );
    QCOMPARE( mTranslator->translateText( QString( "Aarne" ) ), QString( "22763" ) );
    QCOMPARE( mTranslator->translateText( QString( "Bertta" ) ), QString( "237882" ) );
    QCOMPARE( mTranslator->translateText( QString( "Celsius" ) ), QString( "2357487" ) );
    QCOMPARE( mTranslator->translateText( QString( "Daavid" ) ), QString( "322843" ) );
    QCOMPARE( mTranslator->translateText( QString( "Eemeli" ) ), QString( "336354" ) );
    QCOMPARE( mTranslator->translateText( QString( "Faarao" ) ), QString( "322726" ) );
    QCOMPARE( mTranslator->translateText( QString( "Gideon" ) ), QString( "443366" ) );
    QCOMPARE( mTranslator->translateText( QString( "Heikki" ) ), QString( "434554" ) );
    QCOMPARE( mTranslator->translateText( QString( "Iivari" ) ), QString( "448274" ) );
    QCOMPARE( mTranslator->translateText( QString( "Jussi" ) ), QString( "58774" ) );
    QCOMPARE( mTranslator->translateText( QString( "Kalle" ) ), QString( "52553" ) );
    QCOMPARE( mTranslator->translateText( QString( "Lauri" ) ), QString( "52874" ) );
    QCOMPARE( mTranslator->translateText( QString( "Matti" ) ), QString( "62884" ) );
    QCOMPARE( mTranslator->translateText( QString( "Niilo" ) ), QString( "64456" ) );
    QCOMPARE( mTranslator->translateText( QString( "Otto" ) ), QString( "6886" ) );
    QCOMPARE( mTranslator->translateText( QString( "Paavo" ) ), QString( "72286" ) );
    QCOMPARE( mTranslator->translateText( QString( "Kuu" ) ), QString( "588" ) );
    QCOMPARE( mTranslator->translateText( QString( "Risto" ) ), QString( "74786" ) );
    QCOMPARE( mTranslator->translateText( QString( "Sakari" ) ), QString( "725274" ) );
    QCOMPARE( mTranslator->translateText( QString( "Tyyne" ) ), QString( "89963" ) );
    QCOMPARE( mTranslator->translateText( QString( "Urho" ) ), QString( "8746" ) );
    QCOMPARE( mTranslator->translateText( QString( "Vihtori" ) ), QString( "8448674" ) );
    QCOMPARE( mTranslator->translateText( QString( "Wiski" ) ), QString( "94754" ) );
    QCOMPARE( mTranslator->translateText( QString( "Yrjö" ) ), QString( "9756" ) );
    QCOMPARE( mTranslator->translateText( QString( "Tseta" ) ), QString( "87382" ) );
    QCOMPARE( mTranslator->translateText( QString( "Åke" ) ), QString( "253" ) );
    QCOMPARE( mTranslator->translateText( QString( "Äiti" ) ), QString( "2484" ) );
    QCOMPARE( mTranslator->translateText( QString( "Öljy" ) ), QString( "6559" ) );
    QCOMPARE( mTranslator->translateText( QString( "Adam" ) ), QString( "2326" ) );
    QCOMPARE( mTranslator->translateText( QString( "Bertil" ) ), QString( "237845" ) );
    QCOMPARE( mTranslator->translateText( QString( "Cesar" ) ), QString( "23727" ) );
    QCOMPARE( mTranslator->translateText( QString( "David" ) ), QString( "32843" ) );
    QCOMPARE( mTranslator->translateText( QString( "Erik" ) ), QString( "3745" ) );
    QCOMPARE( mTranslator->translateText( QString( "Filip" ) ), QString( "34547" ) );
    QCOMPARE( mTranslator->translateText( QString( "Gustav" ) ), QString( "487828" ) );
    QCOMPARE( mTranslator->translateText( QString( "Helge" ) ), QString( "43543" ) );
    QCOMPARE( mTranslator->translateText( QString( "Ivar" ) ), QString( "4827" ) );
    QCOMPARE( mTranslator->translateText( QString( "Johan" ) ), QString( "56426" ) );
    QCOMPARE( mTranslator->translateText( QString( "Kalle" ) ), QString( "52553" ) );
    QCOMPARE( mTranslator->translateText( QString( "Ludvig" ) ), QString( "583844" ) );
    QCOMPARE( mTranslator->translateText( QString( "Martin" ) ), QString( "627846" ) );
    QCOMPARE( mTranslator->translateText( QString( "Niklas" ) ), QString( "645527" ) );
    QCOMPARE( mTranslator->translateText( QString( "Olof" ) ), QString( "6563" ) );
    QCOMPARE( mTranslator->translateText( QString( "Petter" ) ), QString( "738837" ) );
    QCOMPARE( mTranslator->translateText( QString( "Quintus" ) ), QString( "7846887" ) );
    QCOMPARE( mTranslator->translateText( QString( "Rudolf" ) ), QString( "783653" ) );
    QCOMPARE( mTranslator->translateText( QString( "Sigurd" ) ), QString( "744873" ) );
    QCOMPARE( mTranslator->translateText( QString( "Tore" ) ), QString( "8673" ) );
    QCOMPARE( mTranslator->translateText( QString( "Urban" ) ), QString( "87226" ) );
    QCOMPARE( mTranslator->translateText( QString( "Viktor" ) ), QString( "845867" ) );
    QCOMPARE( mTranslator->translateText( QString( "Wilhelm" ) ), QString( "9454356" ) );
    QCOMPARE( mTranslator->translateText( QString( "Xerxes" ) ), QString( "937937" ) );
    QCOMPARE( mTranslator->translateText( QString( "Yngve" ) ), QString( "96483" ) );
    QCOMPARE( mTranslator->translateText( QString( "Zäta" ) ), QString( "9282" ) );
    QCOMPARE( mTranslator->translateText( QString( "Ärlig" ) ), QString( "27544" ) );
    QCOMPARE( mTranslator->translateText( QString( "Östen" ) ), QString( "67836" ) );
    QCOMPARE( mTranslator->translateText( QString( "Alpha" ) ), QString( "25742" ) );
    QCOMPARE( mTranslator->translateText( QString( "Bravo" ) ), QString( "27286" ) );
    QCOMPARE( mTranslator->translateText( QString( "Charlie" ) ), QString( "2427543" ) );
    QCOMPARE( mTranslator->translateText( QString( "Delta" ) ), QString( "33582" ) );
    QCOMPARE( mTranslator->translateText( QString( "Echo" ) ), QString( "3246" ) );
    QCOMPARE( mTranslator->translateText( QString( "Foxtrot" ) ), QString( "3698768" ) );
    QCOMPARE( mTranslator->translateText( QString( "Golf" ) ), QString( "4653" ) );
    QCOMPARE( mTranslator->translateText( QString( "Hotel" ) ), QString( "46835" ) );
    QCOMPARE( mTranslator->translateText( QString( "India" ) ), QString( "46342" ) );
    QCOMPARE( mTranslator->translateText( QString( "Juliet" ) ), QString( "585438" ) );
    QCOMPARE( mTranslator->translateText( QString( "Kilo" ) ), QString( "5456" ) );
    QCOMPARE( mTranslator->translateText( QString( "Lima" ) ), QString( "5462" ) );
    QCOMPARE( mTranslator->translateText( QString( "Mike" ) ), QString( "6453" ) );
    QCOMPARE( mTranslator->translateText( QString( "November" ) ), QString( "66836237" ) );
    QCOMPARE( mTranslator->translateText( QString( "Oscar" ) ), QString( "67227" ) );
    QCOMPARE( mTranslator->translateText( QString( "Papa" ) ), QString( "7272" ) );
    QCOMPARE( mTranslator->translateText( QString( "Quebec" ) ), QString( "783232" ) );
    QCOMPARE( mTranslator->translateText( QString( "Romeo" ) ), QString( "76636" ) );
    QCOMPARE( mTranslator->translateText( QString( "Sierra" ) ), QString( "743772" ) );
    QCOMPARE( mTranslator->translateText( QString( "Tanga" ) ), QString( "82642" ) );
    QCOMPARE( mTranslator->translateText( QString( "Uniform" ) ), QString( "8643676" ) );
    QCOMPARE( mTranslator->translateText( QString( "Vârlan" ) ), QString( "827526" ) );
    QCOMPARE( mTranslator->translateText( QString( "Victor" ) ), QString( "842867" ) );
    QCOMPARE( mTranslator->translateText( QString( "Whiskey" ) ), QString( "9447539" ) );
    QCOMPARE( mTranslator->translateText( QString( "Xray" ) ), QString( "9729" ) );
    QCOMPARE( mTranslator->translateText( QString( "Yankee" ) ), QString( "926533" ) );
    QCOMPARE( mTranslator->translateText( QString( "Zulu" ) ), QString( "9858" ) );
    
    QString uni;
    uni.append(QChar(0x0219));
    uni.append(QChar(0x4E0F));
    QString result("7");
    
    QCOMPARE(mTranslator->translateText(uni), result);
    
    QString uni1;
    uni1.append(QChar(0x4E0E));
    uni1.append(QChar(0x4E0F));
    QString result1("");
    QCOMPARE(mTranslator->translateText(uni1), result1);
    
    QString result2;
    QCOMPARE(mTranslator->translateText(uni1), result2);

}

void UT_LogsPredictiveLatin12KeyTranslator::testPreparePattern()
{

    QCOMPARE( mTranslator->preparePattern( QString( "1234567890" ) ), QString( "1234567890" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "+1234567890" ) ) ,QString( "*1234567890" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "*1234567890" ) ) ,QString( "*1234567890" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "#1234567890" ) ), QString( "#1234567890" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "++##" ) ), QString( "**##" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "-+##" ) ), QString( "1*##" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "/+##" ) ), QString( "1*##" ) );
    QCOMPARE( mTranslator->preparePattern( QString( "*+##" ) ), QString( "**##" ) );
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testSplitPattern()
{
    QString car;
    QString cdr;
    
    mTranslator->splitPattern( QString( "122" ), car, cdr );
    QCOMPARE( car, QString("122" ) );
    QCOMPARE( cdr, QString("" ) );
    
    mTranslator->splitPattern( QString( "01220" ), car, cdr );
    QCOMPARE( car, QString("01220" ) );
    QCOMPARE( cdr, QString("" ) );
    
    mTranslator->splitPattern( QString( "0122020" ), car, cdr );
    QCOMPARE( car, QString("0122" ) );
    QCOMPARE( cdr, QString("20" ) );
    
    mTranslator->splitPattern( QString( "012200020" ), car, cdr );
    QCOMPARE( car, QString("0122" ) );
    QCOMPARE( cdr, QString("20" ) );
    
    mTranslator->splitPattern( QString( "0122050020" ), car, cdr );
    QCOMPARE( car, QString("0122" ) );
    QCOMPARE( cdr, QString("50020" ) );
    
    mTranslator->splitPattern( QString( "00000" ), car, cdr );
    QCOMPARE( car, QString("00000" ) );
    QCOMPARE( cdr, QString("" ) );
    
    mTranslator->splitPattern( QString( "0" ), car, cdr );
    QCOMPARE( car, QString("0" ) );
    QCOMPARE( cdr, QString("" ) );

    mTranslator->splitPattern( QString( "" ), car, cdr );
    QCOMPARE( car, QString("" ) );
    QCOMPARE( cdr, QString("" ) );
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testTrimPattern()
{
    QString pattern;
    
    pattern = "1";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "1") );
    
    pattern = "122";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "122") );
    
    pattern = "0122";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "122") );
    
    pattern = "0122";
    QCOMPARE( mTranslator->trimPattern( pattern, true ), QString( "0122") );
    
    pattern = "1220";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "122") );
    
    pattern = "1220";
    QCOMPARE( mTranslator->trimPattern( pattern, true ), QString( "122") );
    
    pattern = "01220";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "122") );
    
    pattern = "01220";
    QCOMPARE( mTranslator->trimPattern( pattern, true ), QString( "0122") );
    
    pattern = "012020";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "1202") );
    
    pattern = "012020";
    QCOMPARE( mTranslator->trimPattern( pattern, true ), QString( "01202") );

    pattern = "000120200";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "1202") );
    
    pattern = "00012000200";
    QCOMPARE( mTranslator->trimPattern( pattern , false), QString( "120002") );
    
    pattern = "0001200024500";
    QCOMPARE( mTranslator->trimPattern( pattern , false), QString( "12000245") );
    
    pattern = "000";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "0") );

    pattern = "0";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "0") );
    
    pattern = "";
    QCOMPARE( mTranslator->trimPattern( pattern, false ), QString( "") );
    
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testPatternTokens()
{
    
    QCOMPARE( mTranslator->patternTokens( QString("122" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("122" ) )[0], QString("122" ) );
    QCOMPARE( mTranslator->patternTokens( QString("01220" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("01220" ) )[0], QString("01220" ) );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) ).length(), 2 );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) )[0], QString("1" ) );
    QCOMPARE( mTranslator->patternTokens( QString("102" ) )[1], QString("2" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) ).length(), 2 );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) )[0], QString("001" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200" ) )[1], QString("200" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200564760635" ) ).length(), 2 );
    QCOMPARE( mTranslator->patternTokens( QString("0010200564760635" ) )[0], QString("001" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0010200564760635" ) )[1], QString("200564760635" ) );
    QCOMPARE( mTranslator->patternTokens( QString("0" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("0" ) )[0], QString("0" ) );
    QCOMPARE( mTranslator->patternTokens( QString("000" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("000" ) )[0], QString("000" ) );
    QCOMPARE( mTranslator->patternTokens( QString("001" ) ).length(), 1 );
    QCOMPARE( mTranslator->patternTokens( QString("001" ) )[0], QString("001" ) );
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testHasPatternSeparators()
{
    QVERIFY( !mTranslator->hasPatternSeparators( QString("122" ) ) );
    QVERIFY( mTranslator->hasPatternSeparators( QString("1022" ) ) );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("0010200" ) ), 5 );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("0000" ) ), 4 );
    QCOMPARE( mTranslator->hasPatternSeparators( QString("345345" ) ), 0 );
    
}

void UT_LogsPredictiveLatin12KeyTranslator::testTranslateChar()
{
    bool ok;
    
    QCOMPARE( mTranslator->translateChar( QChar('+'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('*'), ok ), QString('*') );
    
    QCOMPARE( mTranslator->translateChar( QChar('#'), ok ), QString('#') );
    QCOMPARE( mTranslator->translateChar( QChar('0'), ok ), QString('0') );
    QCOMPARE( mTranslator->translateChar( QChar('a'), ok ), QString('2') );
    QCOMPARE( mTranslator->translateChar( QChar('A'), ok ), QString('2') );
    QCOMPARE( mTranslator->translateChar( QChar('1'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('ä'), ok ), QString('2') );
    QCOMPARE( mTranslator->translateChar( QChar('Ä'), ok ), QString('2') );
    QCOMPARE( mTranslator->translateChar( QChar('?'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('!'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('\''), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('"'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('-'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('('), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar(')'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('@'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('/'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar(':'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar('_'), ok ), QString('1') );
    QCOMPARE( mTranslator->translateChar( QChar(';'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('&'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('%'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('='), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('<'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('>'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('['), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar(']'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('{'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('}'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('\\'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('~'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('^'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('¡'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('¿'), ok ), QString('*') );
    QCOMPARE( mTranslator->translateChar( QChar('|'), ok ), QString('*') );
    
    // Romanian unicode character "sh"
    QCOMPARE( mTranslator->translateChar( QChar(0x0219), ok ), QString('7') );
    // Chinese unicode character
    QChar chn(0x4E0F);
    QCOMPARE( mTranslator->translateChar( QChar(0x4E0F), ok ), QString() );
    QVERIFY( !ok );


}

void UT_LogsPredictiveLatin12KeyTranslator::testEncodes()
{
    QVERIFY( mTranslator->encodes( QString( "Zulu" ) ) );
    QVERIFY( mTranslator->encodes( QString( "Öistämö" ) ) );
    
    //china
    QString uni1;
    uni1.append(QChar(0x4E0E));
    uni1.append(QChar(0x4E0F));
    QVERIFY( !mTranslator->encodes( uni1) );
    
    const int ucsize = 9;
    const QChar thaiName2[] = {0x0E01,0x0E06,0x0E2F,0x0E0E,0x0E14,0x0E19,0x0E30,0x0E23,0x0E2A };
    QVERIFY( !mTranslator->encodes( QString( thaiName2,ucsize ) ) );
    
}


