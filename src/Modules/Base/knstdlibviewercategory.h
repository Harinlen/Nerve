#ifndef KNSTDLIBVIEWERCATEGORY_H
#define KNSTDLIBVIEWERCATEGORY_H

#include <QList>

#include "knstdlibviewercategorybutton.h"

#include "knlibviewercategory.h"

class QBoxLayout;
class QMouseEvent;
class QPaintEvent;
class QSignalMapper;
class KNStdLibViewerCategory : public KNLibViewerCategory
{
    Q_OBJECT
public:
    explicit KNStdLibViewerCategory(QWidget *parent = 0);
    void addCategory(const QPixmap &icon, const QString &category);
    void setText(const int &index, const QString &text);
    void setCurrentIndex(const int &index);
    int currentIndex() const;

signals:

public slots:
    void moveLeft();
    void moveRight();
    void enabledMoving();

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void categoryClicked(int index);
    void changeBackground(int frameData);

private:
    QColor m_backgroundColor;
    QBoxLayout *m_layout;
    QPalette m_palette;
    QSignalMapper *m_categoryClicked;
    QTimeLine *m_mouseIn, *m_mouseOut;

    QList<KNStdLibViewerCategoryButton *> m_categories;
    int m_currentCategory=-1;
    bool m_moving=false;
};

#endif // KNSTDLIBVIEWERCATEGORY_H
