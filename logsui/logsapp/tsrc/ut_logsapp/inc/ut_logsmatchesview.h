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
#ifndef UT_LOGSMATCHESVIEW_H
#define UT_LOGSMATCHESVIEW_H


#include <QObject>
#include "logsviewmanagerstub.h"

class LogsMatchesView;
class LogsComponentRepository;
class HbMainWindow;


class UT_LogsMatchesView : public QObject
{
    Q_OBJECT
     
private slots:

/*
 * In addition, there are four private slots that are not treated as testfunctions. 
 * They will be executed by the testing framework and can be used to initialize and clean up 
 * either the entire test or the current test function.
 * 
 * initTestCase() will be called before the first testfunction is executed.
 * cleanupTestCase() will be called after the last testfunction was executed.
 * init() will be called before each testfunction is executed.
 * cleanup() will be called after every testfunction.
*/
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();


private slots: //test methods
     
    void testConstructor();
    void testActivated();
    void testDeactivated();
    void testCallKeyPressed();
    void testVideoCallToCurrentNum();
    void testSendMessageToCurrentNum();
    void testSaveNumberInDialpadToContacts();
    void testDialpadEditorTextChanged();
    void testDialpadClosed();
    void testDialpadOpened();
    void testModel();
    void testUpdateWidgetsSizeAndLayout();
    void testHandleOrientationChanged();
    void testUpdateEmptyListWidgetsVisibility();
    void testUpdateAddContactButton();
    
private:
 
    LogsMatchesView* mMatchesView;
    LogsComponentRepository* mRepository;
    HbMainWindow* mMainWindow;
    LogsViewManagerStub* mViewManager;
};

#endif //UT_LOGSMATCHESVIEW_H
