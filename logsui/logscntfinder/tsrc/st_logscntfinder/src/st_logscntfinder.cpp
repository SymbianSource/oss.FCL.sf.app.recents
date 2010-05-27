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
#include "st_logscntfinder.h"
#include "logscntfinder.h"
#include "testresultxmlparser.h"

#include <qtcontacts.h>
#include <QtTest/QtTest>
#include <hbinputsettingproxy.h>

void st_LogsCntFinder::initTestCase()
{
}

void st_LogsCntFinder::cleanupTestCase()
{
    
}


void st_LogsCntFinder::init()
{   
    m_finder = 0;
    //open symbian database
    m_manager = new QContactManager("symbian");
    m_finder = new LogsCntFinder(*m_manager);
    QVERIFY(m_finder);
    
    // Remove all contacts from the database
    QList<QContactLocalId> cnt_ids = m_manager->contactIds();
    qDebug() << "contacts now before deleting" << cnt_ids.count();
    
    QVERIFY(m_manager->removeContacts(&cnt_ids, 0));
    cnt_ids = m_manager->contactIds();
    QCOMPARE(cnt_ids.count(), 0);

    for (int i = 0; i < 10; ++i) {
        m_finder->predictiveSearchQuery( QString::number(i) );
        QCOMPARE( m_finder->resultsCount(), 0 );
    }
}

void st_LogsCntFinder::cleanup()
{
    delete m_manager;
    m_manager = 0;
    delete m_finder;
    m_finder = 0;
    HbInputLanguage eng( QLocale::English );
    HbInputSettingProxy::instance()->setGlobalInputLanguage( eng );
    
}

void st_LogsCntFinder::createContacts()
{
    /*Create contacts in Contacts DB for keymap testing
		Stefann    Yadira
		Jonn         Ennon
		Maria-Zola     Jones
		Levis         Augustin Zi
		Nancy       Csoma
		Olga          Baraniktestteste
		Petter       Harhai
		Queen      Fesko
		Rose        Galisin
		Sasha      Dofzin
		Tisha       Iatzkovits
		Wilda       Lazar
		Una Vivi   Kantsak
		*/
    createOneContact( QString("Stefann"), QString("Yadira "), QString("932472398") );
    createOneContact( QString("Jonn"), QString("Ennon"), QString("932472398") );
    createOneContact( QString("Maria-Zola"), QString("Jones"), QString("932472398") );
    createOneContact( QString("Levis"), QString("Augustin Zi"), QString("932472398") );
    createOneContact( QString("Nancy"), QString("Csoma"), QString("932472398") );
    createOneContact( QString("Olga"), QString("Baraniktestteste"), QString("932472398") );
    createOneContact( QString("Petter"), QString("Harhai"), QString("932472398") );
    createOneContact( QString("Queen"), QString("Fesko"), QString("932472398") );
    createOneContact( QString("Rose"), QString("Galisin"), QString("932472398") );
    createOneContact( QString("Sasha"), QString("Dofzin"), QString("932472398") );
    createOneContact( QString("Tisha"), QString("Iatzkovits"), QString("932472398") );
    createOneContact( QString("Wilda"), QString("Lazar"), QString("932472398") );
    createOneContact( QString("Una Vivi"), QString("Kantsak"), QString("932472398") );
    
   
    int contactsCount = m_manager->contactIds().count();
    QCOMPARE(contactsCount, 13);
    qDebug() << "st_LogsCntFinder::createContacts_testKeymap. created " << contactsCount << " contacts";

    
}

void st_LogsCntFinder::createThaiContacts()
{
    /*Create contacts in Contacts DB

		*/
		
    const QChar thaiFName1[] = {0x0E06,0x0E0A}; // map:23
    const QChar thaiLName1[] = {0x0E0E,0x0E14,0x0E19,0x0E1E,0x0E23,0x0E2A };//map 456789
    
    QString first ( thaiFName1, 2 );
    QString last ( thaiFName1, 6 );
    createOneContact( first, last, QString("932472398") );
		
    const QChar thaiFName2[] = {0x0E30,0x0E0A};//#3, 
    const QChar thaiLName2[] = {0x0E2F,0x0E14,0x0E19,0x0E1E,0x0E23,0x0E2A };//*56789
    
    QString first2 ( thaiFName1, 2 );
    QString last2 ( thaiFName1, 6 );
    createOneContact( first2, last2, QString("932472398") );
		
		
   
    int contactsCount = m_manager->contactIds().count();
    QCOMPARE(contactsCount, 2);
    qDebug() << "st_LogsCntFinder::createThaiContacts_ThaiSearch. created " << contactsCount << " contacts";

    
}

void st_LogsCntFinder::createHistoryEvents()
{
  createLogEvent( QString("Stefann Albert"), QString("Fedrernn"), QString("932472398") );
  createLogEvent( QString("Jonn"), QString("Lennon"), QString("932472398") );
  createLogEvent( QString("Dim-Petter"), QString("Jones"), QString("932472398") );
  
}

void st_LogsCntFinder::createLogEvent(QString firstname, QString lastname, 
                                      QString phnumber)
{
  LogsCntEntryHandle* dummy = 0;
  
  LogsCntEntry* logEvent = new LogsCntEntry( *dummy, 0 );
  logEvent->setFirstName( firstname );
  logEvent->setLastName( lastname );
  logEvent->setPhoneNumber( phnumber );
  
  m_finder->insertEntry(0, logEvent );
  
  
}


void st_LogsCntFinder::createOneContact(QString firstname, QString Lastname, 
                                                 QString phnumber)
{
    //Currenlty we can only fetch firstname,lastname,companyname and sip/email/phone from the databse
    // so create contact with only these details
    QContact phonecontact;
    // Stefann Fedrernn +02644424423 ste.Fed@nokia.com
    // Contact details
    QContactName contactName;
    contactName.setFirstName(firstname);
    contactName.setLastName(Lastname);
    phonecontact.saveDetail(&contactName);
    
    QContactPhoneNumber number;
    number.setContexts("Home");
    number.setSubTypes("Mobile");
    number.setNumber(phnumber);
    phonecontact.saveDetail(&number);
    
    m_manager->saveContact(&phonecontact);
    qDebug() << "st_LogsCntFinder::createOneContact done";
}



//
// Tests
//

void st_LogsCntFinder::testPredictiveThaiSearchQuery()
{
    HbInputLanguage thai( QLocale::Thai );
    HbInputSettingProxy::instance()->setGlobalInputLanguage( thai );
    		  
    createThaiContacts();

		//There is no recent call in logs UI
    //case 1
    /*m_finder->predictiveSearchQuery( QString("2") );
    QCOMPARE( m_finder->resultsCount(), 1 );*/
    qDebug() << "-- 2 query starts --";
    m_finder->predictiveSearchQuery( QString("2") );
    qDebug() << "found " << m_finder->resultsCount() << " matches:";
 
    qDebug() << "-- 4 query starts --";
    m_finder->predictiveSearchQuery( QString("4") );
    qDebug() << "found " << m_finder->resultsCount() << " matches:";
        qDebug() << "-- 402 query starts --";
    m_finder->predictiveSearchQuery( QString("402") );
    qDebug() << "found " << m_finder->resultsCount() << " matches:";
        qDebug() << "-- *4 query starts --";
    m_finder->predictiveSearchQuery( QString("*4") );
    qDebug() << "found " << m_finder->resultsCount() << " matches:";
        qDebug() << "-- *402### query starts --";
    m_finder->predictiveSearchQuery( QString("*402###") );
    qDebug() << "found " << m_finder->resultsCount() << " matches:";

    /*m_finder->predictiveSearchQuery( QString("4") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    m_finder->predictiveSearchQuery( QString("402") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    m_finder->predictiveSearchQuery( QString("*4") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    m_finder->predictiveSearchQuery( QString("*402###") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    m_finder->predictiveSearchQuery( QString("*4***00002###") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    */
    
    
}


// Test basic predictive search, all records with names starting letters "J, K, L" are matched
void st_LogsCntFinder::testPredictiveSearchQuery()
{
    createContacts();
		
		//There is no recent call in logs UI
    //case 1
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 5 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Maria-Zola"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Jones"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Jonn"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Ennon"));*/

    m_finder->predictiveSearchQuery( QString("566") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 5 );
    
    //case 2
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 5 );
    //see all of the matched results
    //results == for(int i =0;i<f->resultCount();i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Maria-Zola"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Jones"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Jonn"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Ennon"));
    QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Una Vivi"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Kantsak"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Wilda"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Lazar"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Levis"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Augustin Zi"));
    */

    m_finder->predictiveSearchQuery( QString("566") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 5 );
    
    //case 3
    m_finder->predictiveSearchQuery( QString("6") );
    QCOMPARE( m_finder->resultsCount(), 3 );
    //see all of matched results
    //results == for(int i =0;i<f->resultCount();i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Maria-Zola"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Jones"));
    QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Nancy"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Csoma"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Olga"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Baraniktestteste"));*/

    m_finder->predictiveSearchQuery( QString("69") );
    QCOMPARE( m_finder->resultsCount(), 0 );
    m_finder->predictiveSearchQuery( QString("692") );
    QCOMPARE( m_finder->resultsCount(), 0 );
    m_finder->predictiveSearchQuery( QString("6") );
    QCOMPARE( m_finder->resultsCount(), 3 );
    
    //case 4
    
    m_finder->predictiveSearchQuery( QString("2") );
    QCOMPARE( m_finder->resultsCount(), 3 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Levis"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Augustin Zi"));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString(""));*/

    m_finder->predictiveSearchQuery( QString("20") );
    QCOMPARE( m_finder->resultsCount(), 3 );
    m_finder->predictiveSearchQuery( QString("209") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    m_finder->predictiveSearchQuery( QString("20") );
    QCOMPARE( m_finder->resultsCount(), 3 );
    m_finder->predictiveSearchQuery( QString("2") );
    QCOMPARE( m_finder->resultsCount(), 3 );
       
}

/* Test itut keymap predictive search, checking that press key "2", records with names starting letters "A, B, C" are matched;
Press key "3", records with names starting letters "D,E,F" are matched;
Press key "4", records with names starting letters "G,H,I" are matched;
Press key "5", records with names starting letters "J,K,L" are matched;
Press key "6", records with names starting letters "M,O,N" are matched;
Press key "7", records with names starting letters "P,Q,R,S" are matched;
Press key "8", records with names starting letters "T,U,V" are matched;
Press key "9", records with names starting letters "W,X,Y,Z" are matched;
Press key "0", records with names starting letters "space" etc. are matched;
Press key "1", records with names starting letters "-,Ä,Ö" etc. are matched;
*/
void st_LogsCntFinder::testKeymap()
{
    createContacts();
    const LogsCntEntry* data; 

    for (int i = 2; i < 10; i++)
    {
        m_finder->predictiveSearchQuery( QString::number(i) );
        switch( i ) 
        {
        case 2:
            QCOMPARE( m_finder->resultsCount(), 3 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;     

        case 3:
            QCOMPARE( m_finder->resultsCount(), 3 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;

        case 4:
            QCOMPARE( m_finder->resultsCount(), 3 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;

        case 5:
            QCOMPARE( m_finder->resultsCount(), 5 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;

        case 6:
            QCOMPARE( m_finder->resultsCount(), 3 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;

        case 7:
            QCOMPARE( m_finder->resultsCount(), 5 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(),1 );
            break;

        case 8:
            QCOMPARE( m_finder->resultsCount(), 2 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;

        case 9:
            QCOMPARE( m_finder->resultsCount(), 3 );
            data = &m_finder->resultAt( 0 );
            QCOMPARE( data->firstName().count(), 1 );
            break;
        }
    }
}

/* Test zero query search: 1. zero between "1-9" numbers, then first zero works as "AND" statement; 
2. (multiple) zero at beginning; 3. (multiple) zero at the end; 
4-5. multi-zeros between "1-9" numbers, only the first works as "AND" statement;
6. Query limit is 15, the 16th is ignored, and first 0 works as "AND" statement */


void st_LogsCntFinder::testPredictiveSearchQueryZeroStart()
{
    createContactsForQueryZero();
    
    m_finder->predictiveSearchQuery( QString("00202") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
    m_finder->predictiveSearchQuery( QString("02010") );
    QCOMPARE( m_finder->resultsCount(), 1 );

}
void st_LogsCntFinder::testPredictiveSearchQueryZeroMiddle()
{
    createContactsForQueryZero();
    m_finder->predictiveSearchQuery( QString("566") );
    QCOMPARE( m_finder->resultsCount(), 2 );

    m_finder->predictiveSearchQuery( QString("56603") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    m_finder->predictiveSearchQuery( QString("2003") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
    m_finder->predictiveSearchQuery( QString("2272645837883065") );
    QCOMPARE( m_finder->resultsCount(), 1 );
       
    m_finder->predictiveSearchQuery( QString("200904") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    
}

/* 
Dlice 00202       Qwerty        45789348
#Paula 2003       Augustin Ci   0078945617 
Paula 02010       Ezerty Adam   78945617
Ced               Y,g           +78945617
Jari-Pekka        Baraniktestteste 78945617
*/
void st_LogsCntFinder::createContactsForQueryZero()
{
    createContacts();
    createOneContact( QString("Dlice 00202"), QString("Qwerty"), QString("45789348") );
    createOneContact( QString("#Paula 2003"), QString("Augustin Ci"), QString("0078945617") );
    createOneContact( QString("Paula 02010"), QString("Ezerty Adam"), QString("78945617") );
    createOneContact( QString("Ced"), QString("Y,g"), QString("+78945617") );
    createOneContact( QString("Jari-Pekka"), QString(" "), QString("78945617") );

    int contactsCount = m_manager->contactIds().count();
    QCOMPARE(contactsCount, 18);
}

// Test query limit is 15, the 16th digit is ignored
void st_LogsCntFinder::testPredictiveSearchQueryLimit()
{
    createContacts();

    // 9 digits
    m_finder->predictiveSearchQuery( QString("227264583") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 10 digits
    m_finder->predictiveSearchQuery( QString("2272645837") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 11 digits
    m_finder->predictiveSearchQuery( QString("22726458378") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 12 digits
    m_finder->predictiveSearchQuery( QString("227264583788") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 13 digits
    m_finder->predictiveSearchQuery( QString("2272645837883") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 14 digits
    m_finder->predictiveSearchQuery( QString("22726458378837") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    // 15 digits
    m_finder->predictiveSearchQuery( QString("227264583788378") );
    QCOMPARE( m_finder->resultsCount(), 1 );

    // 16 digits
    m_finder->predictiveSearchQuery( QString("2272645837883783") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Olga"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Baraniktestteste"));
}

//There is recent call in logs, no contacts DB
void st_LogsCntFinder::testPredictiveSearchQueryLogs()
{
    createHistoryEvents();

    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
    //case 1
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString(""));*/

    m_finder->predictiveSearchQuery( QString("533") );
    QCOMPARE( m_finder->resultsCount(), 0 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
        //case 2
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString(""));*/

    m_finder->predictiveSearchQuery( QString("50") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("503") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    m_finder->predictiveSearchQuery( QString("50") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
        
}

//There is recent call in logs, and contacts DB
void st_LogsCntFinder::testPredictiveSearchQueryLogsContacts()
{
    createHistoryEvents();
    createContacts();

    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
    //case 1
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString(""));*/

    m_finder->predictiveSearchQuery( QString("533") );
    QCOMPARE( m_finder->resultsCount(), 0 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    
        //case 2
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    //see half of matched results
    //results == for(int i =0;i<f->resultCount() / 2;i++){f->resultAt(i);}
    /*QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString(""));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString(""));*/

    m_finder->predictiveSearchQuery( QString("50") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("503") );
    QCOMPARE( m_finder->resultsCount(), 1 );
    m_finder->predictiveSearchQuery( QString("50") );
    QCOMPARE( m_finder->resultsCount(), 2 );
    m_finder->predictiveSearchQuery( QString("5") );
    QCOMPARE( m_finder->resultsCount(), 2 );
        
}

void st_LogsCntFinder::testQueryOrder()
{
    createContactsForQueryOrder();

    m_finder->predictiveSearchQuery( QString("7") );
    QCOMPARE( m_finder->resultsCount(), 8 );
    
    QCOMPARE(m_finder->resultAt(0).firstName().at(0).text(), QString("Anna"));
    QCOMPARE(m_finder->resultAt(0).lastName().at(0).text(), QString("Qwerty"));

    QCOMPARE(m_finder->resultAt(1).firstName().at(0).text(), QString("Paula"));
    QCOMPARE(m_finder->resultAt(1).lastName().at(0).text(), QString("Azerty"));

    QCOMPARE(m_finder->resultAt(2).firstName().at(0).text(), QString("Paula"));
    QCOMPARE(m_finder->resultAt(2).lastName().at(0).text(), QString("Qwerty"));

    QCOMPARE(m_finder->resultAt(3).firstName().at(0).text(), QString("Petter"));
    QCOMPARE(m_finder->resultAt(3).lastName().at(0).text(), QString("Harhai"));

    QCOMPARE(m_finder->resultAt(4).firstName().at(0).text(), QString("Queen"));
    QCOMPARE(m_finder->resultAt(4).lastName().at(0).text(), QString("Fesko"));

    QCOMPARE(m_finder->resultAt(5).firstName().at(0).text(), QString("Rose"));
    QCOMPARE(m_finder->resultAt(5).lastName().at(0).text(), QString("Galisin"));

    QCOMPARE(m_finder->resultAt(6).firstName().at(0).text(), QString("Sasha"));
    QCOMPARE(m_finder->resultAt(6).lastName().at(0).text(), QString("Dofzin"));

    QCOMPARE(m_finder->resultAt(7).firstName().at(0).text(), QString("Stefann"));
    QCOMPARE(m_finder->resultAt(7).lastName().at(0).text(), QString("Yadira"));
}

void st_LogsCntFinder::createContactsForQueryOrder()
{
    createContacts();
    createOneContact( QString("Anna"), QString("Qwerty"), QString("45789348") );
    createOneContact( QString("Paula"), QString("Qwerty"), QString("78945617") );
    createOneContact( QString("Paula"), QString("Azerty"), QString("78945617") );

    int contactsCount = m_manager->contactIds().count();
    QCOMPARE(contactsCount, 16);
}

//QTEST_MAIN(st_LogsCntFinder);

/*int main(int argc, char *argv[])
{
    bool promptOnExit(true);
    bool xmlOutput(false);

    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-noprompt") {
            promptOnExit = false;
        }
        if (QString(argv[i]) == "-xml") {
            xmlOutput = true;
        }
    }
    printf("Running tests...\n");

    QApplication app(argc, argv);
    st_LogsCntFinder st_logscntfinder;
    QString resultFileName = "c:/data/others/st_LogsCntFinder";
    resultFileName.append(xmlOutput ? ".xml" : ".txt");
    QStringList args_logsCntFinder("st_logscntfinder");
    if (xmlOutput) 
        args_logsCntFinder.append("-xml");
    args_logsCntFinder << "-v1" << "-o" << resultFileName;
    QTest::qExec(&st_logscntfinder, args_logsCntFinder);

    if (xmlOutput) {
        TestResultXmlParser parser;
        parser.parseAndPrintResults(resultFileName);        
    }
    
    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    
    return 0;
}*/

int main(int argc, char *argv[]) //on HW
{
    QApplication app(argc, argv);
    
    st_LogsCntFinder st_logscntfinder;
    QString resultFileName = "c:/data/others/st_logscntfinder.txt";
    QStringList args_logsCntFinder( "st_logscntfinder");
    args_logsCntFinder << "-o" << resultFileName;
    QTest::qExec(&st_logscntfinder, args_logsCntFinder);
    return 0;   
}
