#ifndef TXKEYERNONEFORM_H
#define TXKEYERNONEFORM_H

#include "txKeyerFormBase.h"
#include <QWidget>



class TxKeyerNoneForm : public TxKeyerFormBase
{
    Q_OBJECT

public:
    explicit TxKeyerNoneForm(QWidget *parent = nullptr);
    ~TxKeyerNoneForm();



private:

};

#endif // TXKEYERNONEFORM_H
