#ifndef KNMUSICVIEWERMENU_H
#define KNMUSICVIEWERMENU_H

#include "../../../Modules/Base/knmenu.h"

class KNMusicViewerMenu : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenu(QWidget *parent = 0);
    void setFilePath(const QString &filePath);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void onActionBrowse();

private:
    enum MusicActions
    {
        Browse,
        Copy,
        MusicActionCount
    };
    QString m_actionTitle[MusicActionCount];
    QAction *m_action[MusicActionCount];
    void createActions();

    QString m_filePath;
};

#endif // KNMUSICVIEWERMENU_H
