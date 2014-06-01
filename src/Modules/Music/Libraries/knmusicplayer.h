#ifndef KNMUSICPLAYER_H
#define KNMUSICPLAYER_H

#include "../../Public/bass/knlibbass.h"

#include <QObject>

class KNMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlayer(QObject *parent = 0);
    void playFile(const QString &fileName);
    void setPosition(const int &position);
    void setVolume(const float &volume);
    KNLibBass *backend();
    void getGraphicData(float *fftData);
    int volume() const;

signals:
    void positionChanged(quint32 position);
    void durationChanged(quint32 duration);
    void reachEndOfMusic();

public slots:
    void play();
    void pause();

private:
    KNLibBass *m_player;
    float m_originalVolume=-1;
};

#endif // KNMUSICPLAYER_H
