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
#include "logsdetailsview.h"
#include "logsdetailsmodel.h"
#include "logscall.h"
#include "logsmessage.h"
#include "logslogger.h"
#include "logscomponentrepository.h"
#include "logsabstractviewmanager.h"
#include "logsdefs.h"
#include "logscontact.h"

//SYSTEM
#include <hblistview.h>
#include <hbaction.h>
#include <hbgroupbox.h>
#include <hbmessagebox.h>
#include <hblabel.h>
#include <dialpad.h>
#include <hblineedit.h>
Q_DECLARE_METATYPE(LogsDetailsModel*)


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
LogsDetailsView::LogsDetailsView( 
    LogsComponentRepository& repository, LogsAbstractViewManager& viewManager )
    : LogsBaseView(LogsDetailsViewId, repository, viewManager),
      mViewName(0),
      mListView(0)
{
    LOGS_QDEBUG( "logs [UI] <-> LogsDetailsView::LogsDetailsView()" );
    
    //TODO: taking away due to toolbar bug. If toolbar visibility changes on view
    //activation, there will be a crash due to previous view effect is playing
    //addViewSwitchingEffects();
}
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
LogsDetailsView::~LogsDetailsView()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::~LogsDetailsView()" );
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::~LogsDetailsView()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::activated(bool showDialer, QVariant args)
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::activated()" );
    //base class handling first
    LogsBaseView::activated(showDialer, args);
   
    LogsDetailsModel* model = qVariantValue<LogsDetailsModel*>(args);
    if ( !model ){
        LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::activated(), no model" );
        return;
    }
    
    if ( mListView && mDetailsModel && mDetailsModel != model ){
        mListView->setModel( 0 );
        delete mDetailsModel;
    }
    mDetailsModel = model;
    
    if (mViewName){
        mViewName->setHeading(mDetailsModel->headerData(0, Qt::Vertical).toString());
    }
    
    if ( mListView ){
        mListView->setModel( mDetailsModel );//ownership not transferred
    }
    
    updateMenu();
    
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::activated()" );
}

// -----------------------------------------------------------------------------
// LogsDetailsView::deactivated
// -----------------------------------------------------------------------------
//
void LogsDetailsView::deactivated()
{
    //base class handling first
    LogsBaseView::deactivated();
    if ( mListView ){
        mListView->setModel( 0 );
    }
    delete mDetailsModel;
    mDetailsModel = 0;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::callKeyPressed()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::callKeyPressed()" );
    if ( !tryCallToDialpadNumber() && mCall ){
        mCall->initiateCallback();
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::callKeyPressed()" );
}

// -----------------------------------------------------------------------------
// LogsDetailsView::initiateVoiceCall
// -----------------------------------------------------------------------------
//
void LogsDetailsView::initiateVoiceCall()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::initiateVoiceCall()" );
    if ( !tryCallToDialpadNumber() && mCall ){
        mCall->call(LogsCall::TypeLogsVoiceCall);
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::initiateVoiceCall()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::initiateVideoCall()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::initiateVideoCall()" );
    if ( !tryCallToDialpadNumber(LogsCall::TypeLogsVideoCall) && mCall ){
        mCall->call(LogsCall::TypeLogsVideoCall);
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::initiateVideoCall()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::sendMessage()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::sendMessage()" );
    if ( !tryMessageToDialpadNumber() && mMessage ){
        mMessage->sendMessage();
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::sendMessage()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::openContact()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::openContact()" );
    if ( mContact ){
        // Use async connection to ensure that model can handle
        // contact operation completion before view
        QObject::connect(mContact, SIGNAL(openCompleted(bool)),
                         this, SLOT(contactActionCompleted(bool)),
                         Qt::QueuedConnection);
        mContact->open();
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::openContact()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::contactActionCompleted(bool modified)
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::contactActionCompleted()" );
    
    if (modified){
        updateMenu();
        if (mViewName){
            mViewName->setHeading(
                    mDetailsModel->headerData(0, Qt::Vertical).toString());
        }
    }
    
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::contactActionCompleted()" );
}

// -----------------------------------------------------------------------------
// LogsDetailsView::handleBackSoftkey
// -----------------------------------------------------------------------------
//
void LogsDetailsView::handleBackSoftkey()
{
    mViewManager.activatePreviousView();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsDetailsView::deleteEventOkAnswer()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::deleteEventOkAnswer()" );
    if (mDetailsModel) {
        mDetailsModel->clearEvent();
        handleBackSoftkey(); 
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::deleteEventOkAnswer()" );
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::initView()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::initView()" );
    //base class handling first
    LogsBaseView::initView();
        
    mViewName = qobject_cast<HbGroupBox*>(
                        mRepository.findWidget( logsGroupboxDetailViewNameId ) );  
    initListWidget();

    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::initView()" );   
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
QAbstractItemModel* LogsDetailsView::model() const
{
    return mDetailsModel;
}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void LogsDetailsView::initListWidget()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::initListWidget()" );
    mListView = qobject_cast<HbListView*> 
                    ( mRepository.findWidget( logsDetailsListWidgetId ) );
    Q_ASSERT_X(mListView != 0, "logs [UI] ", "couldn't find list widget !!");

    mListView->setItemRecycling(true);

    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::initListWidget() " );
}

// -----------------------------------------------------------------------------
// LogsDetailsView::updateMenu
// -----------------------------------------------------------------------------
//
void LogsDetailsView::updateMenu()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::updateMenu()" );
    if ( !mDetailsModel ) {
        return;
    }
    
    updateCall( mDetailsModel->index(0) );
    updateMessage( mDetailsModel->index(0) );
    updateContact( mDetailsModel->index(0) );
    
    HbAction* voiceCallAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsDetailsViewVoiceCallMenuActionId ) );
    HbAction* videoCallAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsCommonVideoCallMenuActionId ) );
    HbAction* internetCallAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsDetailsViewInternetCallMenuActionId ) );
    HbAction* messageAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsCommonMessageMenuActionId ) );
    HbAction* addToContactsAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsCommonAddToContactsMenuActionId ) );
    HbAction* openContactAction = qobject_cast<HbAction*>( 
            mRepository.findObject( logsDetailsOpenContactMenuActionId ) );
    
    bool voiceCallAvailable(false);
    bool videoCallAvailable(false);
    bool internetCallAvailable(false); // Currently always disabled
    if (mCall) {
        foreach( LogsCall::CallType callType, mCall->allowedCallTypes() ){
            if ( callType == LogsCall::TypeLogsVoiceCall ){
                voiceCallAvailable = true;
            } else if ( callType == LogsCall::TypeLogsVideoCall ) {
                videoCallAvailable = true;
            }
        }
    }
    
    bool contactCanBeAdded(false);
    bool contactCanBeOpened(false);
    if ( isDialpadInput() ){
        // Contact addition will be done using input field number
        contactCanBeAdded = true;
        // No need for voice call option in options menu as voice dialling
        // to dialpad num is possible from green button
        voiceCallAvailable = false;
    } else if (mContact){
        if (mContact->allowedRequestType() == LogsContact::TypeLogsContactSave){
            contactCanBeAdded = true;
        }
        if (mContact->allowedRequestType() == LogsContact::TypeLogsContactOpen){
            contactCanBeOpened = true;
        }
    }
    
    toggleActionAvailability(voiceCallAction, voiceCallAvailable);
    toggleActionAvailability(videoCallAction, videoCallAvailable);
    toggleActionAvailability(internetCallAction, internetCallAvailable);
    toggleActionAvailability(messageAction, mMessage);
    toggleActionAvailability(addToContactsAction, contactCanBeAdded);
    toggleActionAvailability(openContactAction, contactCanBeOpened);
    
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::updateMenu()" );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LogsDetailsView::dialpadEditorTextChanged()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::dialpadEditorTextChanged()" );
    if ( !tryMatchesViewTransition() ) {
        updateMenu();
        updateCallButton();  
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::dialpadEditorTextChanged()" );
}

// -----------------------------------------------------------------------------
// LogsDetailsView::updateWidgetsSizeAndLayout
// -----------------------------------------------------------------------------
//
void LogsDetailsView::updateWidgetsSizeAndLayout()
{
    LOGS_QDEBUG( "logs [UI] -> LogsDetailsView::updateWidgetsSizeAndLayout()" );
    if ( mListView ) {
        updateMenu();
        updateListLayoutName(*mListView, true);
        updateListSize();
    }
    LOGS_QDEBUG( "logs [UI] <- LogsDetailsView::updateWidgetsSizeAndLayout()" );
}
