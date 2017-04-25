#ifndef STARTFORM_H
#define STARTFORM_H

#include <QWidget>

namespace Ui {
class StartForm;
}

class StartForm : public QWidget
{
    Q_OBJECT

public:
    explicit StartForm(QWidget *parent = 0);
    ~StartForm();

private:
    Ui::StartForm *ui;
};

#endif // STARTFORM_H
