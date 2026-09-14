#include "QtUtils.h"
#include "kstcallsframe.h"
#include "kstmainwindow.h"
#include "kstasactiveframe.h"
#include "kstplanesframe.h"
#include "ui_kstasactiveframe.h"

inline const QString STATUS_INDICATOR_DISCONNECT_STYLE = QString("background-color: Gainsboro;\n");
inline const QString STATUS_INDICATOR_CONNECT_STYLE = QString("background-color: Sandybrown;\n");

KSTASActiveFrame::KSTASActiveFrame(QWidget *parent)
    : MinosPanel(parent)
    , ui(new Ui::KSTASActiveFrame)
{
    ui->setupUi(this);
    ui->ASActivecb->setChecked(mainWindow->getASActive());
    on_ASActivecb_stateChanged(mainWindow->getASActive());
    asStatusIndicatorToggle(false);
}

KSTASActiveFrame::~KSTASActiveFrame()
{
    delete ui;
}

void KSTASActiveFrame::on_FontChanged()
{

}

void KSTASActiveFrame::setASBands(QVector<const char *> ASBandStrings)
{
    for(auto const &s: QASCONST(ASBandStrings))
    {
        ui->asBandCombo->addItem(AirScoutLink::tr(s));
    }
    ui->asBandCombo->setCurrentIndex(getASActiveBand());
}

void KSTASActiveFrame::on_ASActivecb_stateChanged(int state)
{
    asStatusIndicatorToggle(false);
    mainWindow->do_ASActive(state);
    if (mainWindow->started)
    {
        if (mainWindow->asl && mainWindow->getASActive())
        {
            for (auto const &kstuser: QASCONST(*mainWindow->callVector))
            {
                kstuser->planes.clear();
                kstuser->planeResponseSeen = false;
            }
            mainWindow->callVectorChanged = true;
            emit mainWindow->kstCallsFrame->kstCallModel.dataChanged(
                mainWindow->kstCallsFrame->kstCallModel.index(0, ecscAirscout)
                , mainWindow->kstCallsFrame->kstCallModel.index(mainWindow->callVector->size(), ecscAirscout));

            mainWindow->asl->clearWatchList();
            mainWindow->userCallTimerTimer();
        }

        QSettings settings(mainWindow->iniName, QSettings::IniFormat);

        settings.setValue("ASActive", state != 0);

        mainWindow->showPlanesFrame(state != 0);

        mainWindow->kstCallsFrame->showAircout(state);
    }
}
void KSTASActiveFrame::do_asBandCombo_currentIndexChanged(int band)
{
    if (mainWindow->started)
    {
        if (mainWindow->asl && getASActive())
        {
            for (auto const &kstuser: QASCONST(*mainWindow->callVector))
            {
                kstuser->planes.clear();
                kstuser->planeResponseSeen = false;
            }
            mainWindow->callVectorChanged = true;
            emit mainWindow->kstCallsFrame->kstCallModel.dataChanged(mainWindow->kstCallsFrame->kstCallModel.index(0, ecscAirscout),
                        mainWindow->kstCallsFrame->kstCallModel.index(mainWindow->callVector->size(), ecscAirscout));

            mainWindow->userCallTimerTimer();
        }
        QSettings settings(mainWindow->iniName, QSettings::IniFormat);

        settings.setValue("ASActiveBand", band);
    }
}

void KSTASActiveFrame::on_asBandCombo_currentIndexChanged(int band)
{
    do_asBandCombo_currentIndexChanged(band);
}

bool KSTASActiveFrame::getASActive() const
{
    bool ret = ui->ASActivecb->isChecked();
    return ret;
}
void KSTASActiveFrame::setASActive(bool s)
{
    ui->ASActivecb->setChecked(s);
}

ASBand KSTASActiveFrame::getASActiveBand() const
{
    ASBand b = mainWindow->getASActiveBand();
    return b;
}
void KSTASActiveFrame::asStatusIndicatorToggle(bool on)
{
    if (on)
    {
        ui->asStatusIndicator->setStyleSheet(STATUS_INDICATOR_CONNECT_STYLE);
    }
    else
    {
        ui->asStatusIndicator->setStyleSheet(STATUS_INDICATOR_DISCONNECT_STYLE);
    }
}