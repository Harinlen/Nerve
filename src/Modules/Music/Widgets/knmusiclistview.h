#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include <QTreeView>

class QTimer;
class QTimeLine;
class KNMusicDetailTooltip;
class KNMusicListViewHeader;
class KNMusicListView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);
    virtual void resetHeader();
    void moveToFirst(const int &logicalHeaderIndex);

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);
    void onSectionVisibleChanged(const int &index,
                                 const bool &visible);
    void onItemActived(const QModelIndex &index);
    void onItemEntered(const QModelIndex &index);
    void updateTooltipAndShow();

private:
    KNMusicListViewHeader *m_headerWidget;
    KNMusicDetailTooltip *m_detailTooltip;
    QTimer *m_detailTooltipShower;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor;
    QPalette m_palette;
    QModelIndex m_detailIndex;
};

#endif // KNMUSICLISTVIEW_H
