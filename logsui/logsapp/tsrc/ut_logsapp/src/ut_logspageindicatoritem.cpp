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

//USER
#include "ut_logspageindicatoritem.h"
#include "logspageindicatoritem.h"

//SYSTEM
#include <QtTest/QtTest>
#include <QGraphicsColorizeEffect>

void UT_LogsPageIndicatorItem::initTestCase()
{
}

void UT_LogsPageIndicatorItem::cleanupTestCase()
{
}


void UT_LogsPageIndicatorItem::init()
{
    mIndicatorItem = new LogsPageIndicatorItem();
}

void UT_LogsPageIndicatorItem::cleanup()
{
    delete mIndicatorItem;
}

void UT_LogsPageIndicatorItem::testConstructorDestructor()
{
    QVERIFY(!mIndicatorItem->mIsActive);
    QVERIFY(mIndicatorItem->graphicsEffect());
    
    delete mIndicatorItem;
    mIndicatorItem = 0;
    mIndicatorItem = new LogsPageIndicatorItem(true);
    QVERIFY(mIndicatorItem->mIsActive);
}

void UT_LogsPageIndicatorItem::testSetActive()
{
    //active state not changed, nothing to do
    QVERIFY(!mIndicatorItem->mIsActive);
    mIndicatorItem->setActive(false);
    QVERIFY(!mIndicatorItem->mIsActive);
    
    //setting active, animation started
    mIndicatorItem->setActive(true);
    QVERIFY(mIndicatorItem->mIsActive);
    QVERIFY(mIndicatorItem->graphicsEffect());
    QVERIFY(mIndicatorItem->graphicsEffect()->isEnabled());
    
    //setting inactive
    mIndicatorItem->setActive(false);
    QVERIFY(!mIndicatorItem->mIsActive);
    QVERIFY(mIndicatorItem->graphicsEffect());
    QVERIFY(mIndicatorItem->graphicsEffect()->isEnabled());
}

void UT_LogsPageIndicatorItem::testAnimationFinnished()
{
    QVERIFY(mIndicatorItem->graphicsEffect());
    mIndicatorItem->graphicsEffect()->setEnabled(true);
    mIndicatorItem->animationFinished();
    QVERIFY(!mIndicatorItem->graphicsEffect()->isEnabled());
}
