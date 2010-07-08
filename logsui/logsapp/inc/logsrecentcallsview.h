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
#ifndef LOGSRECENTCALLSVIEW_H
#define LOGSRECENTCALLSVIEW_H

#include "logsfilter.h"
#include "logsbaseview.h"
#include <hbscrollarea.h>

class HbListView;
class HbLabel;
class LogsComponentRepository;
class LogsModel;
class HbAbstractViewItem;
class LogsAbstractViewManager;
class LogsEffectHandler;
class HbGroupBox;
class LogsMatchesModel;
class LogsPageIndicator;


/**
 * 
 */
class LogsRecentCallsView : public LogsBaseView
{
    Q_OBJECT
    friend class UT_LogsRecentCallsView;
    friend class UT_LogsViewManager;
    
public:
    explicit LogsRecentCallsView( LogsComponentRepository& repository,
                                  LogsAbstractViewManager& viewManager );
    ~LogsRecentCallsView();

public: // From LogsBaseView
    
    virtual void activated(bool showDialer, QVariant args);
    virtual void deactivated();
    virtual bool isExitAllowed();
    virtual QString saveActivity(QDataStream& serializedActivity, QVariantHash& metaData);
    virtual QVariant loadActivity(
        const QString& activityId, QDataStream& serializedActivity, QVariantHash& metaData);
    
public slots:
    
    void handleExit();
    void markingCompleted(int err);
    
protected slots: // from LogsBaseView 
    
    virtual void handleBackSoftkey();
    void openDialpad();
    virtual void dialpadEditorTextChanged();
    void changeFilter(HbAction* action);
    virtual void updateEmptyListWidgetsVisibility();
    virtual void updateWidgetsSizeAndLayout();
    
private slots:

    void clearList();
    void clearListAnswer(int action);
    void toggleContactSearch();
    void updateView(LogsServices::LogsView view);
    void leftFlick();
    void rightFlick();
    void dissappearByFadingComplete();
    void dissappearByMovingComplete();
    void appearByMovingComplete();
    bool markMissedCallsSeen();
    
private: // from LogsBaseView
    
    virtual void initView();
    virtual QAbstractItemModel* model() const;
    virtual LogsAbstractModel* logsModel() const;
    virtual HbListView* listView() const;
    virtual void updateCallButton();
    
private:
    
    void addStringsToMap();
    void initListWidget();
    void updateFilter(LogsFilter::FilterType type);
    void updateViewName();
    void updateContextMenuItems(LogsServices::LogsView view);
    LogsFilter::FilterType getFilter(LogsServices::LogsView view);  
    void changeView(LogsServices::LogsView view);
    void updateMenu();
    void handleMissedCallsMarking();
    
    //from HbWidget
    void gestureEvent(QGestureEvent *event);
    
    int getListItemTextWidth();
    
    
private:
    
    HbGroupBox* mViewName;   //not owned
    HbListView* mListView;//not owned
    LogsFilter* mFilter;  //owned
    
    QMap<LogsBaseView::LogsViewMap, QString>   mTitleMap;
    QMap<LogsServices::LogsView, LogsBaseView::LogsViewMap>   mConversionMap;	
    
    LogsModel* mModel;

    LogsServices::LogsView mCurrentView;
    LogsServices::LogsView mAppearingView;
    bool mMoveLeftInList;
    LogsEffectHandler* mEffectHandler;
    int mListViewX;
    int mEmptyListLabelX;
    LogsMatchesModel* mMatchesModel; 
    bool mMarkingMissedAsSeen;
    LogsPageIndicator* mPageIndicator;
    bool mFirstActivation;
    
    HbScrollArea::ScrollBarPolicy mListScrollBarPolicy;
    
};

#endif // LOGSRECENTCALLSVIEW_H
