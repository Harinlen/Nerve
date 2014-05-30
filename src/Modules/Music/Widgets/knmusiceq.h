#ifndef KNMUSICEQ_H
#define KNMUSICEQ_H

#include <QWidget>

#include "../../Public/bass/knlibbass.h"

class QSlider;
class QSignalMapper;
class QLabel;
class QLineEdit;
class KNLibBass;
class KNMusicEQ : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicEQ(KNLibBass *backend, QWidget *parent = 0);
    void setBackend(KNLibBass *backend);

signals:

public slots:

private slots:
    void setEQParam(const int &index);
    void setTuneValue(const int &index);

private:
    KNLibBass *m_bassBackend;
    QSignalMapper *m_tuneRelease, *m_textChange;
    QLineEdit *m_equalizerEdit[KNLibBass::EqualizerCount];
    QSlider *m_equalizerTune[KNLibBass::EqualizerCount];
    QLabel *m_equalizerCaption[KNLibBass::EqualizerCount];
};

#endif // KNMUSICEQ_H
