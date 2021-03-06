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
#include "ut_logscommondata.h"
#include "logscommondata.h"
#include "logsconfigurationparams.h"
#include <xqsettingsmanager.h>
#include <hbcolorscheme.h>

#include <QtTest/QtTest>

void UT_LogsCommonData::initTestCase()
{
}

void UT_LogsCommonData::cleanupTestCase()
{
}


void UT_LogsCommonData::init()
{
}

void UT_LogsCommonData::cleanup()
{
}

void UT_LogsCommonData::testGetInstance()
{
    QVERIFY( &LogsCommonData::getInstance() != 0 );
    QVERIFY( &LogsCommonData::getInstance() == &LogsCommonData::getInstance() );
}

void UT_LogsCommonData::testGetContactManager()
{
    QVERIFY( &LogsCommonData::getInstance().contactManager() != 0 );
    QVERIFY( &LogsCommonData::getInstance().contactManager() == &LogsCommonData::getInstance().contactManager() );
}

void UT_LogsCommonData::testFreeCommonData()
{
    LogsCommonData* comData = &LogsCommonData::getInstance();
    QVERIFY( comData != 0 );
    LogsCommonData::freeCommonData();
}

void UT_LogsCommonData::testCurrentConfiguration()
{
    LogsConfigurationParams params;
    params.setListItemTextWidth(400);
    params.setLocaleChanged(true);
    LogsCommonData::getInstance().updateConfiguration(params);
    LogsConfigurationParams& test = LogsCommonData::getInstance().currentConfiguration();
    QVERIFY( test.listItemTextWidth() == 400 );
    QVERIFY( test.localeChanged() );
}


void UT_LogsCommonData::testClearMissedCallsCounter()
{
    // Value is changed
//    LogsCommonData::freeCommonData();
    XQSettingsManager::mFailed = false;
    XQSettingsManager::mCurrentVal = 5;
    QVERIFY( LogsCommonData::getInstance().clearMissedCallsCounter() == 0 );
    QVERIFY( XQSettingsManager::mCurrentVal == 0 );
    
    // No need to change value as it is already zero
    QVERIFY( LogsCommonData::getInstance().clearMissedCallsCounter() == 0 );
    QVERIFY( XQSettingsManager::mCurrentVal == 0 );
    
    // Fails with some error
    XQSettingsManager::mFailed = true;
    XQSettingsManager::mCurrentVal = 5;
    QVERIFY( LogsCommonData::getInstance().clearMissedCallsCounter() != 0 );
    QVERIFY( XQSettingsManager::mCurrentVal == 5 );
}

void UT_LogsCommonData::testPredictiveSearchStatus()
{    

    // Predictive search fetching failed
    LogsCommonData::freeCommonData();
    XQSettingsManager::mFailed = true;
    XQSettingsManager::mCurrentVal = 1;
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == -1 );
    QVERIFY( LogsCommonData::getInstance().predictiveSearchStatus() == -1 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == -1 );

    // Predictive search value asked from settings manager
    XQSettingsManager::mFailed = false;
    QVERIFY( LogsCommonData::getInstance().predictiveSearchStatus() == 1 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 1 );
    
    // Predictive search asked again, local value returned
    XQSettingsManager::mCurrentVal = 2;
    QVERIFY( LogsCommonData::getInstance().predictiveSearchStatus() == 1 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 1 );
}

void UT_LogsCommonData::testSetPredictiveSearch()
{
    // Value change is not allowed
    LogsCommonData::getInstance().mPredictiveSearchStatus = 0;
    QVERIFY( LogsCommonData::getInstance().setPredictiveSearch(true) == -1 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 0 );
    
    // Value changed, setting predictive search On
    LogsCommonData::getInstance().mPredictiveSearchStatus = 2;
    QVERIFY( LogsCommonData::getInstance().setPredictiveSearch(true) == 0 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 1 );
    QVERIFY( XQSettingsManager::mCurrentVal == 1 );
    
    // Value changed, setting predictive search Off
    LogsCommonData::getInstance().mPredictiveSearchStatus = 1;
    QVERIFY( LogsCommonData::getInstance().setPredictiveSearch(false) == 0 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 2 );
    QVERIFY( XQSettingsManager::mCurrentVal == 2 );

    // Fails with some error
    XQSettingsManager::mFailed = true;
    QVERIFY( LogsCommonData::getInstance().setPredictiveSearch(true) == -1 );
    QVERIFY( LogsCommonData::getInstance().mPredictiveSearchStatus == 2 );
    QVERIFY( XQSettingsManager::mCurrentVal == 2 );
}

void UT_LogsCommonData::testRefreshData()
{
    // Theme change not pending
    LogsCommonData& ct = LogsCommonData::getInstance();
    QSignalSpy spy( &ct, SIGNAL(commonDataChanged()) ); 
    ct.mCompressed = true;
    ct.mHighlightColorStart.clear();
    ct.refreshData();
    QCOMPARE( spy.count(), 0 );
    QVERIFY( !ct.mCompressed );
    QVERIFY( !ct.mHighlightColorStart.isEmpty() );
    
    // Pending theme change
    ct.mPendingThemeChange = true;
    ct.mCompressed = true;
    ct.mHighlightColorStart.clear();
    ct.refreshData();
    QCOMPARE( spy.count(), 1 );
    QVERIFY( !ct.mCompressed );
    QVERIFY( !ct.mHighlightColorStart.isEmpty() );
    
    // No action when already refreshed
    ct.refreshData();
    QCOMPARE( spy.count(), 1 );
    QVERIFY( !ct.mCompressed );
    QVERIFY( !ct.mHighlightColorStart.isEmpty() );
}

void UT_LogsCommonData::testCompressData()
{
    LogsCommonData& ct = LogsCommonData::getInstance();
    ct.mCompressed = false;
    ct.compressData();
    QVERIFY( ct.mCompressed );
}

void UT_LogsCommonData::testHandleThemeChange()
{
    LogsCommonData& ct = LogsCommonData::getInstance();
    QSignalSpy spy( &ct, SIGNAL(commonDataChanged()) ); 
    
    // Theme changed when not compressed
    ct.mCompressed = false;
    ct.mPendingThemeChange = false;
    ct.handleThemeChange();
    QCOMPARE( spy.count(), 1 );
    QVERIFY( !ct.mPendingThemeChange );
    
    // Theme changed when compressed
    spy.clear();
    ct.mCompressed = true;
    ct.handleThemeChange();
    QCOMPARE( spy.count(), 0 );
    QVERIFY( ct.mPendingThemeChange );
}

void UT_LogsCommonData::testHighlightColor()
{
    LogsCommonData& ct = LogsCommonData::getInstance();
    QColor highlight = HbColorScheme::color("qtc_lineedit_marker_normal");
    QColor color = HbColorScheme::color("qtc_lineedit_selected");
    QString start = QString("<span style=\"background-color: %1; color: %2\">")
              .arg(highlight.name().toUpper())
              .arg(color.name().toUpper());
    QCOMPARE( ct.highlightStart(), start );
    QCOMPARE( ct.highlightEnd(), QString("</span>") );
}

