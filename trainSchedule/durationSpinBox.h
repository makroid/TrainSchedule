#ifndef DURATIONSPINBOX_H
#define DURATIONSPINBOX_H

#include <QSpinBox>

class QRegExpValidator;

class DurationSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    DurationSpinBox(QWidget* parent=0);

protected:
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;
    QValidator::State validate(QString &input, int &pos) const;

private:
    QRegExpValidator* validator;
    QRegExp timeFormat;

    void toSixty(int durInSecs, int& h, int &m, int& s) const;
};

#endif // DURATIONSPINBOX_H
