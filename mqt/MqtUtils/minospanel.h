#ifndef MINOSPANEL_H
#define MINOSPANEL_H

#include <QFrame>

class MinosPanel : public QFrame
{
    Q_OBJECT
public:
    explicit MinosPanel(QWidget *parent = nullptr);

    virtual void setPanelFont();
    int getFontsize() const;

    QFont getPanelFont() const;
    void setPanelFont(QFont &f);

    void setFontsize(int newFontsize);

private:
    int fontsize = 100;
    QFont panelFont;
private slots:
    void onSetFont();
};

#endif // MINOSPANEL_H
