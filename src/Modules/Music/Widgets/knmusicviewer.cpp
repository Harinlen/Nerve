#include <QAbstractItemModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QModelIndex>

#include <QStyleFactory>

#include <QDebug>

#include "knmusiclistview.h"
#include "knmusicartistview.h"
#include "knmusicalbumview.h"

#include "../Libraries/knmusicsortmodel.h"
#include "../Libraries/knmusicartistmodel.h"
#include "../Libraries/knmusicalbummodel.h"
#include "../Libraries/knmusicgenremodel.h"
#include "../Libraries/knmusiccategorydetailmodel.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    setContentsMargins(0,0,0,0);
    setAcceptDrops(true);

    m_listViewModel=new KNMusicSortModel(this);
    m_artistModel=new KNMusicArtistModel(this);
    m_albumModel=new KNMusicAlbumModel(this);
    m_genreModel=new KNMusicGenreModel(this);

    m_libraryView=new KNMusicListView(this);
    m_libraryView->setModel(m_listViewModel);
    connect(m_libraryView, &KNMusicListView::requireOpenUrl,
            this, &KNMusicViewer::onActionListviewOpenUrl);
    connect(m_libraryView, &KNMusicListView::requireShowContextMenu,
            this, &KNMusicViewer::onActionLibraryViewShowContextMenu);

    m_artistView=new KNMusicArtistView(this);
    connect(m_artistView, &KNMusicArtistView::requireOpenUrl,
            this, &KNMusicViewer::onActionArtistOpenUrl);
    connect(m_artistView, &KNMusicArtistView::requireShowContextMenu,
            this, &KNMusicViewer::onActionArtistShowContextMenu);
    m_artistView->setModel(m_artistModel);

    m_albumView=new KNMusicAlbumView(this);
    m_albumView->setModel(m_albumModel);

    m_genreView=new KNMusicArtistView(this);
    connect(m_genreView, &KNMusicArtistView::requireOpenUrl,
            this, &KNMusicViewer::onActionGenreOpenUrl);
    connect(m_genreView, &KNMusicArtistView::requireShowContextMenu,
            this, &KNMusicViewer::onActionGenreShowContextMenu);
    m_genreView->setModel(m_genreModel);

    m_artistDetails=new KNMusicCategoryDetailModel(this);
    m_artistDetails->setFilterKeyColumn(KNMusicGlobal::Artist);
    m_artistDetails->setCategoryModel(m_artistModel);
    m_artistView->setDetailModel(m_artistDetails);
    connect(m_artistDetails, &KNMusicCategoryDetailModel::requireDetailSizeChange,
            m_artistView, &KNMusicArtistView::onActionDetailSizeChange);

    m_genreDetails=new KNMusicCategoryDetailModel(this);
    m_genreDetails->setFilterKeyColumn(KNMusicGlobal::Genre);
    m_genreDetails->setCategoryModel(m_genreModel);
    m_genreView->setDetailModel(m_genreDetails);
    connect(m_genreDetails, &KNMusicCategoryDetailModel::requireDetailSizeChange,
            m_genreView, &KNMusicArtistView::onActionDetailSizeChange);

    addCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                m_categoryCaption[Songs],
                m_libraryView);
    addCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                m_categoryCaption[Artists],
                m_artistView);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                m_albumView);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                m_genreView);
}

void KNMusicViewer::setModel(QAbstractItemModel *model)
{
    m_listViewModel->setSourceModel(model);
    m_artistModel->setSourceModel(model);
    m_artistDetails->setSourceModel(model);
    m_albumModel->setSourceModel(model);
    m_genreModel->setSourceModel(model);
    m_genreDetails->setSourceModel(model);
    m_libraryView->resetHeader();
    m_artistView->resetHeader();
    m_genreView->resetHeader();
    m_sourceModel=model;
}

void KNMusicViewer::retranslate()
{
    m_categoryCaption[Songs]=tr("Songs");
    m_categoryCaption[Artists]=tr("Artists");
    m_categoryCaption[Albums]=tr("Albums");
    m_categoryCaption[Genres]=tr("Genres");
}

void KNMusicViewer::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewer::resort()
{
    m_artistView->resort();
    m_albumModel->sort(0);
    m_genreView->resort();
}

void KNMusicViewer::showIn(KNMusicGlobal::MusicCategory category,
                           const QModelIndex &index)
{
    switch(category)
    {
    case KNMusicGlobal::SongsView:
        m_libraryView->setCurrentIndex(m_listViewModel->mapFromSource(index));
        break;
    case KNMusicGlobal::ArtistView:
        m_artistView->selectCategoryItem(m_sourceModel->data(m_sourceModel->index(index.row(),
                                                                                  KNMusicGlobal::Artist)).toString());
        m_artistView->selectItem(index);
        break;
    case KNMusicGlobal::AlbumView:
        break;
    case KNMusicGlobal::GenreView:
        break;
    }
    setCategoryIndex(category);
}

void KNMusicViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void KNMusicViewer::dropEvent(QDropEvent *event)
{
    emit requireAnalysisUrls(event->mimeData()->urls());
}

void KNMusicViewer::onActionLibraryViewShowContextMenu(const QPoint &position,
                                                       const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_listViewModel->mapToSource(index));
}

void KNMusicViewer::onActionArtistShowContextMenu(const QPoint &position,
                                                  const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_artistDetails->mapToSource(index));
}

void KNMusicViewer::onActionGenreShowContextMenu(const QPoint &position,
                                                 const QModelIndex &index)
{
    emit requireShowContextMenu(position,
                                m_genreDetails->mapToSource(index));
}

void KNMusicViewer::onActionListviewOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_listViewModel->mapToSource(index));
}

void KNMusicViewer::onActionArtistOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_artistDetails->mapToSource(index));
}

void KNMusicViewer::onActionGenreOpenUrl(const QModelIndex &index)
{
    emit requireOpenUrl(m_genreDetails->mapToSource(index));
}
