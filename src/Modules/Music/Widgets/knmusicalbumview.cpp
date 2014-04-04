#include <QDebug>

#include <QScrollBar>
#include <QPen>
#include <QMouseEvent>
#include <QIcon>
#include <QPainter>
#include <QPaintEvent>

#include "knmusicalbumview.h"

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    verticalScrollBar()->setRange(0, 0);
    horizontalScrollBar()->setPageStep((m_gridHeight+m_spacing)<<1);
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    int pointLine=
            (verticalScrollBar()->value()+point.y())/(m_gridHeight+m_spacing),
        pointColumn=
            point.x()/(m_spacing+m_gridWidth);
    return model()->index(pointLine*m_maxColumnCount+pointColumn,
                          0,
                          rootIndex());
}

void KNMusicAlbumView::scrollTo(const QModelIndex &index,
                                QAbstractItemView::ScrollHint hint)
{
    if(!index.isValid())
    {
        return;
    }
    int atTopPosition=index.row()/m_maxColumnCount*(m_gridHeight+m_spacing);
    switch(hint)
    {
    case QAbstractItemView::PositionAtTop:
        break;
    case QAbstractItemView::PositionAtCenter:
        atTopPosition-=(height()-m_gridHeight-m_spacing)/2;
        break;
    case QAbstractItemView::PositionAtBottom:
        atTopPosition-=height()+m_gridHeight+m_spacing;
        break;
    case QAbstractItemView::EnsureVisible:
        break;
    }
    verticalScrollBar()->setValue(atTopPosition);
    update();
}

QRect KNMusicAlbumView::visualRect(const QModelIndex &index) const
{
    if(index.isValid())
    {
        int minimalIndex=verticalScrollBar()->value()/(m_gridHeight+m_spacing);
        if(minimalIndex>index.row())
        {
            int;
            QRect rect;
            return rect;
        }
    }
    return QRect();
}

void KNMusicAlbumView::setModel(QAbstractItemModel *model)
{
    QAbstractItemView::setModel(model);
    updateGeometries();
}

void KNMusicAlbumView::dataChanged(const QModelIndex &topLeft,
                                   const QModelIndex &bottomRight)
{
    QAbstractItemView::dataChanged(topLeft, bottomRight);
    viewport()->update();
}

void KNMusicAlbumView::updateGeometries()
{
    verticalScrollBar()->setRange(0, qMax(0,
                                          m_lineCount*(m_gridHeight+m_spacing)+m_spacing-height()));
    verticalScrollBar()->setPageStep((m_gridHeight+m_spacing)<<1);
}

void KNMusicAlbumView::paintEvent(QPaintEvent *event)
{
    QItemSelectionModel *selections = selectionModel();
    QStyleOptionViewItem option = viewOptions();
    QBrush background = option.palette.base();
    QPen foreground(option.palette.color(QPalette::WindowText));
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRect rect=event->rect();
    painter.setRenderHint(QPainter::Antialiasing);
    if(autoFillBackground())
    {
        painter.fillRect(event->rect(), background);
    }
    painter.setPen(foreground);

    int realWidth=rect.width()-m_spacing,
        realMinimumWidth=m_gridMinimumWidth+m_spacing;
    if(realWidth<realMinimumWidth)
    {
        m_maxColumnCount=1;
        m_gridWidth=realWidth-m_spacing;
    }
    else
    {
        m_maxColumnCount=realWidth/realMinimumWidth;
        m_gridWidth=realWidth/m_maxColumnCount-m_spacing;
    }
    int albumIndex=0, albumCount=model()->rowCount(),
            currentRow=0, currentColumn=0,
            currentLeft=m_spacing, currentTop=m_spacing;
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;

    painter.translate(0, -verticalScrollBar()->value());
    int skipLineCount=verticalScrollBar()->value()/(m_gridHeight+m_spacing),
        drawnHeight=0, maxDrawnHeight=height()+m_gridHeight+m_spacing;
    currentRow+=skipLineCount;
    currentTop+=(m_spacing+m_gridHeight)*skipLineCount;
    albumIndex=skipLineCount*m_maxColumnCount;
    m_firstVisibleIndex=albumIndex;
    while(albumIndex < albumCount && drawnHeight < maxDrawnHeight)
    {
        QModelIndex index=model()->index(albumIndex, 0, rootIndex());
        if(selections->isSelected(index))
        {
            qDebug()<<"Selected";
        }
        QRect currentRect=QRect(currentLeft,
                                currentTop,
                                m_gridWidth,
                                m_gridHeight);
        paintAlbum(&painter,
                   currentRect,
                   index);
        currentColumn++;
        if(currentColumn==m_maxColumnCount)
        {
            currentColumn=0;
            currentRow++;
            currentLeft=m_spacing;
            currentTop+=m_spacing+m_gridHeight;
            drawnHeight+=m_spacing+m_gridHeight;
        }
        else
        {
            currentLeft+=m_spacing+m_gridWidth;
        }
        albumIndex++;
    }
    updateGeometries();
}

int KNMusicAlbumView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int KNMusicAlbumView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool KNMusicAlbumView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

QModelIndex KNMusicAlbumView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                                         Qt::KeyboardModifiers)
{
    QModelIndex current = currentIndex();
    viewport()->update();
    return current;
}

void KNMusicAlbumView::setSelection(const QRect &,
                                    QItemSelectionModel::SelectionFlags command)
{
    ;
}

QRegion KNMusicAlbumView::visualRegionForSelection(const QItemSelection &selection) const
{
    QRegion region;
    return region;
}

void KNMusicAlbumView::mousePressEvent(QMouseEvent *e)
{
    m_pressedIndex=indexAt(e->pos());
    QAbstractItemView::mousePressEvent(e);
}

void KNMusicAlbumView::mouseReleaseEvent(QMouseEvent *e)
{
     QAbstractItemView::mouseReleaseEvent(e);
     if(m_pressedIndex==indexAt(e->pos()) &&
        m_pressedIndex.isValid())
     {
         qDebug()<<visualRect(m_pressedIndex);
     }
}

void KNMusicAlbumView::paintAlbum(QPainter *painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    //To draw the album art.
    QIcon currentIcon=model()->data(index, Qt::DecorationRole).value<QIcon>();
    int sizeParam=qMin(rect.width(), rect.height());
    QRect albumArtRect=QRect(rect.x(),rect.y(),sizeParam,sizeParam);
    painter->drawPixmap(albumArtRect,
                        currentIcon.pixmap(sizeParam, sizeParam));
    painter->drawRect(albumArtRect);

    //To draw the text.
    int textTop=rect.y()+sizeParam+5;
    painter->drawText(rect.x(),
                      textTop,
                      rect.width(),
                      rect.height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      model()->data(index).toString());
    textTop+=fontMetrics().height();
    QColor penBackup=painter->pen().color();
    painter->setPen(QColor(128,128,128));
    painter->drawText(rect.x(),
                      textTop,
                      rect.width(),
                      rect.height(),
                      Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignTop,
                      model()->data(index, Qt::UserRole).toString());
    painter->setPen(penBackup);
}

int KNMusicAlbumView::gridMinimumWidth() const
{
    return m_gridMinimumWidth;
}

void KNMusicAlbumView::setGridMinimumWidth(int gridMinimumWidth)
{
    m_gridMinimumWidth = gridMinimumWidth;
}

