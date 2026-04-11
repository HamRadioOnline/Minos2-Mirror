// DMKeyerContainer.h
// Unified container that supports both standalone (combo box) and tabbed modes
#ifndef DMKEYERCONTAINER_H
#define DMKEYERCONTAINER_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QSharedPointer>
#include <QMap>
#include "LoggerContest.h"
#include "txKeyerFactory.h"
#include "txkeyerCommonConstants.h"
#include "dmbuttonframe.h"

class BaseContestLog;
class LoggerContestLog;
class PubSubName;
class Frequency;
class RadioSettingsDialogChangeFlag;

// Forward declarations
class KeyerTab;
class KeyerSettings;

//enum ContainerViewMode {
//    StandaloneMode,  // Single DMButtonFrame with combo box (original behavior)
//    TabbedMode       // Multiple tabs, each with one keyer type
//};

//=============================================================================
// Main container that can switch between standalone and tabbed modes
//=============================================================================
class DMKeyerContainer : public QWidget
{
    Q_OBJECT

public:

    explicit DMKeyerContainer(QWidget *parent = nullptr);
    ~DMKeyerContainer();

    QSharedPointer<KeyerSettings> keyerSettings;
    TxKeyerCommon::TxKeyerCommonSettings txKeyerCommonSettings;

    QString getActiveKeyerName() const;




    // Contest and radio settings (forwarded to active or all frames)
    void setContest(BaseContestLog *contest);
    void setSelectedRadio(PubSubName radio);
    void setRadioIsConnected(bool connected);
    void setFreq(Frequency freq);
    void setMode(const QString &mode);

    // Radio parameter updates
    void setPttEnabled(bool state, PubSubName psn);
    void setPttType(int type, PubSubName psn);

    void setVoiceMemAvail(bool avail, PubSubName psn);
    void setRadioPttState(bool state);
    void setNumVoiceMessages(int numMsgs, PubSubName psn);
    void setRigModel(QString rigModel, PubSubName psn);
    void setCwMemType(int cwMemType, PubSubName psn);
    void setRigVoiceKeyerSupportStopFlag(bool supportStopCmd, PubSubName psn);
    void setRigCwKeyerSupportStopFlag(bool supportStopCmd, PubSubName psn);

    // PC CW Keyer updates
    void setPcCwKeyerComport(QString comportStr);
    void setPcCwKeyerConnectionState(QString stateStr);
    void setPcCwKeyerErrorMsg(QString errorMsg);
    void setPcCwKeyerPttEnabled(QString enabled);
    void setPcCwKeyerTxOnState(QString state);
    void setPcCwKeyerCurrentWpm(QString wpm);

    // Logger radio settings changed
    void logRadioSettingsChanged(QSharedPointer<RadioSettingsDialogChangeFlag> flags);

    // Tab management (only works in TabbedMode)
    void addKeyerTab(const QString &keyerType);
    void removeCurrentTab();
    int getTabCount() const;

    void setErrorMessageDisplayText(const QString errormsg);
    void clearErrorMessageDisplayText();
    void showTemporaryErrorMessage(const QString &msg, int timeoutMs, const QColor &colour);
signals:


    void activeKeyerChanged();
    void pttStatus(bool state);
    void sendFreqControl(Frequency freq);
    void sendWpmToPcCwkeyer(int wpm);
    void sendModeToRadio(const QString &mode);
    void containerModeChanged(TxKeyerCommon::KeyerViewMode newMode);

    void selectedRadioChanged();
    void isRadioConnectedChanged(bool connected);

    void radioFreqChanged(Frequency freq);
    void radioModeChanged(QString mode);


    void contestChanged();
    void pttEnabledChanged();
    void pttTypeChanged();
    void voiceMemAvailChanged();
    void pttStateChanged();
    void numVoiceMessagesChanged();
    void rigModelChanged();
    void cwMemTypeChanged();
    void rigVoiceKeyerSupportStopFlagChanged();
    void rigCwKeyerSupportStopCmdChanged();
    void pcCwKeyerComportChanged();
    void pcCwKeyerConnectionStateChanged();
    void pcCwKeyerErrorMessageChanged();
    void pcCwKeyerPttEnabledChanged();
    void pcCwKeyerTxOnStateChanged();
    void pcCwKeyerCurrentWpmChanged();
    void loggerRadioSettingsChanged();
    void onPttStateChanged();


private slots:
    void onAddKeyerClicked();
    void onTabChanged(int index);
    void onTabCloseRequested(int index);
    void onModeToggleClicked();

    void onKeyerSelectChanged(int index);
private:
    // Mode switching
    void switchToStandaloneMode();
    void switchToTabbedMode();
    void updateViewModeButton();

    // Tab management helpers
    KeyerTab* createKeyerTab(const QString &keyerName);
    void updateActiveTab(KeyerTab *newActiveTab);
    bool isKeyerNameInUse(const QString &keyerName) const;
    QStringList getAvailableKeyerNames() const;


    void setActiveKeyerName(const QString &name);

    // UI components
    QVBoxLayout *mainLayout;
    QHBoxLayout *toolbarLayout;
    QPushButton *addKeyerButton;
    QPushButton *modeToggleButton;
    QComboBox *txKeyerSelect;
    QStackedWidget *stackedWidget;

    QHBoxLayout *keyerErrorMessageLayout;
    KeyerErrorMessageWidget *keyerErrorMessageDisplay;

    QString activeKeyerName;


    // Standalone mode widget
    DMButtonFrame *standaloneFrame;

    // Tabbed mode widgets
    QWidget *tabbedWidget;
    QVBoxLayout *tabbedLayout;
    QTabWidget *tabWidget;

    // Factory for creating keyers
    TxKeyerFactory *txKeyerFactory;

    // Track which keyer names are in use (tabbed mode only)
    QMap<QString, KeyerTab*> keyerNamesInUse;

    // Active tab (tabbed mode only)
    KeyerTab *activeTab;

    // Contest reference
    LoggerContestLog *currentContest;
    void logMessage(QString msg);
    void setContainerViewMode(const KeyerViewMode &viewMode);

};



//=============================================================================
// KeyerTab - Wrapper for DMButtonFrame in tabbed mode
//=============================================================================
class KeyerTab : public QWidget
{
    Q_OBJECT

public:
    explicit KeyerTab(const QString &keyerName, TxKeyerFactory *txKeyerFactory, DMKeyerContainer *keyerContainer,
                      QWidget *parent = nullptr);
    ~KeyerTab();

    // Accessors
    QString getKeyerName() const { return keyerName; }
    DMButtonFrame* getFrame() const { return buttonFrame; }
    bool isActive() const { return active; }

    // Set this tab as active/inactive
    void setActive(bool active);



private:
    QString keyerName;
    bool active;
    DMButtonFrame *buttonFrame;
    QVBoxLayout *layout;
};




class KeyerSettings :  public QObject
{
    Q_OBJECT

public:

    explicit KeyerSettings(){}



    void setContest(LoggerContestLog* contest)
    {
        currentContest = contest;
    }
    LoggerContestLog* getContest() const
    {
        return currentContest;
    }


    void setSelectedRadio(PubSubName selRadio){ selectedRadio = selRadio; }
    PubSubName getSelectedRadio(){ return selectedRadio; }

    void setIsRadioConnected(bool connected){ isRadioConnected = connected; }
    bool getIsRadioConnected(){ return isRadioConnected; }

    void setFreq(Frequency f){ freq = f; }
    Frequency getFreq(){ return freq; }

    void setRadioMode(QString m) { mode = m; }
    QString getRadioMode(){ return mode;}

    void setPttState(bool state){ pttState = state; }
    bool getPttState(){ return pttState; }

    void setPttEnabled(bool state, PubSubName psn)
    {
        allRadioDetails[psn].setPttEnabled(state);
    }


    bool getPttEnabled(PubSubName psn)
    {

        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getPttEnabled();
        }


        return false;
    }

    void setPttType(int type, PubSubName psn)
    {

        allRadioDetails[psn].setPttType(type);

    }

    serialCommonData::MINOS_PTT_TYPES getPttType(PubSubName psn)
    {

        // convert int back to MINOS_PTT_TYPES

        if (allRadioDetails.contains(psn))
        {
            return static_cast<serialCommonData::MINOS_PTT_TYPES>(allRadioDetails[psn].getPttType());
        }


        return serialCommonData::MINOS_PTT_TYPES::PTT_TYPE_NONE;

    }




    void setVoiceMemAvail(bool avail, PubSubName psn)
    {

        allRadioDetails[psn].setVoiceMemAvail(avail);

    }

    bool isVoiceMemAvail(PubSubName psn) const
    {
        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getVoiceMemAvail();
        }

        return false;
    }

    void setNumVoiceMessages(int numMsgs, PubSubName psn)
    {

        allRadioDetails[psn].setNumVoiceMessages(numMsgs);

    }

    // This is max number of voice messages available on a radio
    int getNumVoiceMessages(PubSubName psn)
    {
        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getNumVoiceMessages();
        }
        else
        {
            return MAXIMUM_BUTTONS;
        }
    }

    void setRigVoiceKeyerSupportStopFlag(bool supportStopCmd, PubSubName psn)
    {
        allRadioDetails[psn].setRigVoiceKeyerSupportStopCmd(supportStopCmd);

    }



    bool getRigVoiceKeyerSupportStopFlag(PubSubName psn)
    {

        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getRigVoiceKeyerSupportStopCmd();
        }

        return true;

    }


    void setRigCwKeyerSupportStopFlag(bool supportStopCmd, PubSubName psn)
    {
        allRadioDetails[psn].setRigCwKeyerSupportStopCmd(supportStopCmd);

    }


    bool getRigCwKeyerSupportStopFlag(PubSubName psn)
    {

        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getRigCwKeyerSupportStopCmd();
        }

        return true;

    }

    void setRigModel(QString rigModel, PubSubName psn)
    {
        allRadioDetails[psn].setRigModel(rigModel);
    }

    QString getRigModel(PubSubName psn)
    {
        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getRigModel();
        }

        return "";

    }



    bool isCwMemTypeAvail(PubSubName psn)
    {
        if (allRadioDetails.contains(psn))
        {
           if (allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::KENWOOD
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::YAESU
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::ICOM
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::ELECRAFT
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::FLEX_RADIO
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::FLEX_RADIO_APACHE
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::OPENHPSDR
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::QRPLABS
                || allRadioDetails[psn].getCwMemType() == hamlibData::CW_MEMORY_TYPES::THETIS)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return false;
    }


    void setCwMemType(int cwMemType, PubSubName psn)
    {
        allRadioDetails[psn].setCwMemType(cwMemType);

    }



    int getCwMemType(PubSubName psn)
    {
        if (allRadioDetails.contains(psn))
        {
            return allRadioDetails[psn].getCwMemType();
        }

        return hamlibData::CW_MEMORY_TYPES::NONE;

    }




    void setPcCwKeyerComport(QString comport){ pcCwKeyerComport = comport; }
    QString getPcCwKeyerComport(){ return pcCwKeyerComport; }

    void setPcCwKeyerConnectionState(QString state){ pcCwKeyerConnectionState = state; }
    QString getPcCwKeyerConnectionState(){ return pcCwKeyerConnectionState; }

    void setPcCwKeyerErrorMessage(QString msg){ pcCwKeyerErrorMessage = msg; }
    QString getPcCwKeyerErrorMessage(){ return pcCwKeyerErrorMessage; }

    void setPcCwKeyerPttEnabled(QString enabled){ pcCwKeyerPttEnabled = enabled; }
    QString getPcCwKeyerPttEnabled(){ return pcCwKeyerPttEnabled; }

    void setPcCwKeyerCurrentWpm(QString wpm){ pcCwKeyerCurrentWpm = wpm; }
    QString getPcCwKeyerCurrentWpm(){ return pcCwKeyerCurrentWpm; }

    void setPcCwKeyerTxOnState(QString state){ pcCwKeyerTxOnState = state; }
    QString getPcCwKeyerTxOnState(){ return pcCwKeyerTxOnState; }

    void setLogRadioSettings(QSharedPointer<RadioSettingsDialogChangeFlag> flags_){ flags = flags_;}
    QSharedPointer<RadioSettingsDialogChangeFlag> getLogRadioSettings(){ return flags; }

private:

    LoggerContestLog* currentContest = nullptr;

    // radio settings

    PubSubName selectedRadio;
    bool isRadioConnected;
    Frequency freq;
    QString mode;
    bool pttState = false;

    QMap<PubSubName, RadioDetails> allRadioDetails;

    // PC CW Keyer
    QString pcCwKeyerComport;
    QString pcCwKeyerConnectionState;
    QString pcCwKeyerErrorMessage;
    QString pcCwKeyerPttEnabled;
    QString pcCwKeyerCurrentWpm;
    QString pcCwKeyerTxOnState;

    QSharedPointer<RadioSettingsDialogChangeFlag> flags;


};

#endif // DMKEYERCONTAINER_H
