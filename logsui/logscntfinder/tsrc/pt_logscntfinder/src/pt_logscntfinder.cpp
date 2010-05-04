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
#include "pt_LogsCntFinder.h"
#include "logscntfinder.h"

#include <qtcontacts.h>
#include <QtTest/QtTest>



class PtTest
{
public:
    
    PtTest( const QString& name );
    void execute( LogsCntFinder& engine );
    void calculateMean();
    void calculateVariance();
    void calculateDeviation();
    void print();
    static QString statHeader(); 
    
public:
    
    QString mPattern;
    QList<int> mSamples;
    float mMean;
    float mVariance;
    float mStdDeviation;
    int mMin;
    int mMax;
    int mMinSample;
    int mMaxSample;
    int mResults;

};


PtTest::PtTest( const QString& name )
: mPattern(name),
  mMean(0.0),
  mVariance(0.0),
  mStdDeviation(0.0),
  mMin(0),
  mMax(0),
  mMinSample(0),
  mMaxSample(0),
  mResults(0)
{
    
}

void PtTest::execute( LogsCntFinder& engine )
{
    engine.predictiveSearchQuery( QString("") );
	
    QTime t;
    t.start();
    engine.predictiveSearchQuery( mPattern );
    mSamples.append( t.elapsed());
    mResults += engine.resultsCount();
    //mSamples.append( qrand() / 100000000 );
    //mResults += (100 - mPattern.length() );
    
}


void PtTest::calculateMean()
{
    mMin = 0;
    mMax = 0;
    mMaxSample = 0;
    mMinSample = 0;
    int sum = 0;
    int samplesCount = mSamples.length();
    for( int i = 0; i < samplesCount; i++ ) {
        int sample = mSamples[i];
        if ( sample > mMax ) {
            mMax = sample;
            mMaxSample = i;
        }
        if ( i == 0 || sample < mMin ) {
            mMin = sample;
            mMinSample = i;
        }
        sum += mSamples[i];
    }
    mMean = (sum / samplesCount);
}


void PtTest::calculateVariance()
{
    int samplesCount = mSamples.length();
    float sumdev = 0.0;
    
    for( int i = 0; i < samplesCount; ++i ) {
        float deviation = mSamples[i] - mMean;
        sumdev += (deviation * deviation);
    }
    
    mVariance = (sumdev / (samplesCount - 1));  
    
}

void PtTest::calculateDeviation()
{
    mStdDeviation = sqrt(mVariance);
}

const int collen = 14;


void PtTest::print()
{
    int samplesCount = mSamples.length();
    
    //test  results  samples  mean  variance  deviation  max  min   
    qDebug() << mPattern.leftJustified( collen + 3) +
            QString::number( mResults / samplesCount ).leftJustified( collen ) +
            QString::number( samplesCount ).leftJustified( collen ) +
            QString::number( mMean ).leftJustified( collen ) +
            QString::number( mVariance ).leftJustified( collen ) +
            QString::number( mStdDeviation ).leftJustified( collen ) +
            QString::number( mMax ).leftJustified( collen ) +
            QString::number( mMaxSample ).leftJustified( collen ) +
            QString::number( mMin ).leftJustified( collen ) +
            QString::number( mMinSample ).leftJustified( collen );
}

QString PtTest::statHeader() 
{
    return  QString( "query" ).leftJustified( collen + 3 ) + 
            QString( "results/query" ).leftJustified( collen ) + 
            QString( "samples" ).leftJustified( collen ) + 
            QString( "mean" ).leftJustified( collen ) +
            QString( "variance" ).leftJustified( collen ) +
            QString( "deviation" ).leftJustified( collen ) +
            QString( "max" ).leftJustified( collen ) +
            QString( "maxSample" ).leftJustified( collen ) +
            QString( "min" ).leftJustified( collen ) +
            QString( "minSample" ).leftJustified( collen );

}

void pt_LogsCntFinder::initTestCase()
{
}

void pt_LogsCntFinder::cleanupTestCase()
{
      
}


void pt_LogsCntFinder::init()
{   
    qDebug() << "pt_LogsCntFinder::init start";
    m_finder = 0;
    //open symbian database
    m_manager = new QContactManager("symbian");
    
    //qDebug() << "pt_LogsCntFinder::init remove old contacts";
    // Remove all contacts from the database
    
    QList<QContactLocalId> cnt_ids = m_manager->contactIds();
    qDebug() << "contacts now in db" << cnt_ids.count();
    if ( cnt_ids.count() == 600 ) {
    			QString f("Jack");
    			QString l("Whatever");
          createContact_one_Contact( f,l, QString("03432"));
    	}

	  cnt_ids = m_manager->contactIds();
    qDebug() << "contacts now " << cnt_ids.count();
    
	  
	   
    /*if ( cnt_ids.count() != 600 ) {
    qDebug() << "contacts now before delete" << cnt_ids.count();
      m_manager->removeContacts(&cnt_ids);
      cnt_ids = m_manager->contactIds();
      qDebug() << "contacts now " << cnt_ids.count();
      
      QVERIFY(0 == cnt_ids.count());
      
      //qDebug() << "pt_LogsCntFinder::createContacts";
      createContacts();
      //qDebug() << "pt_LogsCntFinder::createContacts DONE";
      //qDebug() << "contacts now after create" << cnt_ids.count();
    }*/
    
    
    m_finder = new LogsCntFinder( *m_manager );
    
    qDebug() << "pt_LogsCntFinder::init end";
}

void pt_LogsCntFinder::cleanup()
{
		/*m_manager = new QContactManager("symbian");
		QList<QContactLocalId> cnt_ids = m_manager->contactIds();
		m_manager->removeContacts(&cnt_ids);
    cnt_ids = m_manager->contactIds();
    qDebug() << "contacts now " << cnt_ids.count();
      
    QVERIFY(0 == cnt_ids.count());*/
    delete m_manager;
    m_manager = 0;
    delete m_finder;
    m_finder = 0;
}


void pt_LogsCntFinder::createContacts()
{
		//qDebug() << "pt_LogsCntFinder::createContacts";
    QList<QString> firstnamelist;
    QList<QString> Lastnamelist;
    firstnamelist<<"Micheal"<<"Evans"<<"Kris"<<"Xiao"<<"Una Vivi"<<"Wilda"<<"Tisha"<<"Queen"<<"Olga"<<"Maria Zola";
    //Lastnamelist<<"Ribecca"<<"Tina"<<"Bob"<<"George"<<"Anna"<<"Yadira"<<"Lennon"<<"Jones"<<"Augustin Zi"<<"Csoma"<<"Baranik"<<"Harhai";
		Lastnamelist<<"Ribecca"<<"Tina"<<"Bob"<<"George"<<"Anna"<<"Yadira"<<"Lennon"<<"Jones"<<"Augustin Zi"<<"Csoma";

    //for( int z = 0;z<100;z++) {
    for( int z = 0;z<5;z++) {
      for(int i =0; i < firstnamelist.count(); i++) {
        for(int k =0; k < Lastnamelist.count(); k++) {
          createContact_one_Contact(firstnamelist[i], Lastnamelist[k], QString("03432"));
        }
      }
    }
    
    // Empty contact
    //QContact empty;
    //m_manager->saveContact(&empty);
    /*
    QList<QContactLocalId> cnt_ids = m_manager->contactIds();
    cnt_ids = m_manager->contactIds();
    int j = cnt_ids.count();
    QVERIFY( j == 900 );
    */
		//qDebug() << "pt_LogsCntFinder::createContacts DONE";
    
}


void pt_LogsCntFinder::createContact_one_Contact(QString& firstname, QString& Lastname, QString phnumber)
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
    
    //qDebug() << "pt_LogsCntFinder::createContact_one_Contact about to save..";
    
    m_manager->saveContact(&phonecontact);
    //qDebug() << "pt_LogsCntFinder::createContact_one_Contact done";
   
}


void pt_LogsCntFinder::testPredictiveQuery()
{
    const int rowsInDisplay = 10;
    qDebug() << "=>pt_LogsCntFinder::testPredictiveQuery";
    
    QTime t;
    qDebug() << "--- testing query with 5 ---";
    t.start();
    m_finder->predictiveSearchQuery( QString("5") );
    qDebug() << "--- query ends -----";
    qDebug("  Time elapsed:%d ms", t.elapsed());
    qDebug() << "  found " << m_finder->resultsCount() << " matches:";
    
    int results = m_finder->resultsCount();
     
    qDebug() << "-- list 10 matched records start --";
    for( int i=0;(i < rowsInDisplay && i < results);i++) {
 	    QTime t1;
      t1.start();
      const LogsCntEntry& data = m_finder->resultAt( i );
      //qDebug() << "First Name: ";
      for (int j = 0; j < data.firstName().length(); j++) {
        //qDebug() << data.firstName().at(j).text(); 
      }
      //qDebug() << "Last Name: ";
      for (int k = 0; k < data.lastName().length(); k++) {
        //qDebug() << data.lastName().at(k).text();
      }
      //qDebug() << "Phone number: " << data.phoneNumber().text();
      
    qDebug() << "fetched one contact";
    qDebug("  Time elapsed:%d ms", t1.elapsed());
    }
    qDebug() << "-- list 10 matched records end --";
    qDebug("  Time elapsed:%d ms", t.elapsed());

    qDebug() << "<=pt_LogsCntFinder::testPredictiveQuery";
      
    }
    
  


void pt_LogsCntFinder::testExtendedQuery()
{
    qDebug() << "=>pt_LogsCntFinder::testExtendedQuery";
    qDebug() << "<=pt_LogsCntFinder::testPredictiveQuery";

  }
    
void pt_LogsCntFinder::testDiffHitQuery()
{
    qDebug() << "=>pt_LogsCntFinder::testDiffHitQuery";

    QTime t;

    qDebug() << "-- 1 query starts --";
    t.start();
    m_finder->predictiveSearchQuery( QString("1") );
    qDebug() << "-- 1 query ended --"; 
    qDebug("  Time elapsed:%d ms", t.elapsed());
    int results = m_finder->resultsCount();
    qDebug() << "found " << results << " matches:";

    qDebug() << "-- 2 query starts --";
    t.start();
    m_finder->predictiveSearchQuery( QString("2") );
    qDebug() << "-- 2 query ended --"; 
    qDebug("  Time elapsed:%d ms", t.elapsed());
    results = m_finder->resultsCount();
    qDebug() << "found " << results << " matches:";

    qDebug() << "-- 3 query starts --";
    t.start();
    m_finder->predictiveSearchQuery( QString("3") );
    qDebug() << "-- 3 query ended --"; 
    qDebug("  Time elapsed:%d ms", t.elapsed());
    results = m_finder->resultsCount();
    qDebug() << "found " << results << " matches:";

    qDebug() << "-- 6 query starts --";
    t.start();
    m_finder->predictiveSearchQuery( QString("6") );
    qDebug() << "-- 6 query ended --"; 
    qDebug("  Time elapsed:%d ms", t.elapsed());
    results = m_finder->resultsCount();
    qDebug() << "found " << results << " matches:";

    qDebug() << "-- 7 query starts --";
    t.start();
    m_finder->predictiveSearchQuery( QString("7") );
    qDebug() << "-- 7 query ended --"; 
    qDebug("  Time elapsed:%d ms", t.elapsed());
    results = m_finder->resultsCount();
    qDebug() << "found " << results << " matches:";

    qDebug() << "<=pt_LogsCntFinder::testDiffHitQuery";


}

void pt_LogsCntFinder::statistics()
{
    qDebug() << "=> pt_LogsCntFinder::statistics";
    int results = 0;
    int queries = 0;
    int ind = mSamples; //samples
    QTime t;

    QList<PtTest> tests;
    tests.append( PtTest("5") );
    tests.append( PtTest("52") );
    tests.append( PtTest("522") );
    tests.append( PtTest("5220") );
    tests.append( PtTest("522000000000007") );
    tests.append( PtTest("205") );
    tests.append( PtTest("34096") );
    tests.append( PtTest("227264583") );
    tests.append( PtTest("404") );
    tests.append( PtTest("4304") );
    tests.append( PtTest("43043") );
    tests.append( PtTest("227264583788378") );
    tests.append( PtTest("00206") );
    tests.append( PtTest("0705") );
    tests.append( PtTest("12005") );
    tests.append( PtTest("227264583") );
   
    t.start();
    
    while( ind > 0 ) {
        for( int j = 0; j < tests.length(); j++ ) {
            tests[j].execute( *m_finder );
            queries++;
        }
        ind--;
    }
    
    int totalTime = t.elapsed();
    
    
    for( ind = 0; ind < tests.length(); ind++ ) {
        tests[ind].calculateMean();
        tests[ind].calculateVariance();
        tests[ind].calculateDeviation();
        results += tests[ind].mResults;
    }
    
    qDebug("-- Statistics --");
    qDebug("%d queries executed with %d results in %d ms.",
            queries, results, totalTime );
    qDebug() << PtTest::statHeader();
    for( ind = 0; ind < tests.length(); ind++ ) {
        tests[ind].print();
    }
    
    qDebug() << "<= pt_LogsCntFinder::statistics";
}


//QTEST_MAIN(pt_LogsCntFinder); // on Emulator

int main(int argc, char *argv[]) //on HW
{
    int samples = 100;
    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-s") {
            samples = QString(argv[i+1]).toInt();
        }
    }

    QApplication app(argc, argv);
    
    pt_LogsCntFinder pt_logscntfinder( samples );
    QString resultFileName = "c:/data/others/pt_logscntfinder.txt";
    QStringList args_logsCntFinder( "pt_logscntfinder");
    args_logsCntFinder << "-o" << resultFileName;
    QTest::qExec(&pt_logscntfinder, args_logsCntFinder);
    return 0;   
}

