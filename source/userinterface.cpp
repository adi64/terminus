#include "userinterface.h"

#include <memory>
#include <functional>

#include <QDebug>
#include <QQuickView>

#include "game.h"
#include "eventhandler.h"

namespace terminus
{

UserInterface::UserInterface()
    : m_eventHandler(std::unique_ptr<EventHandler>(new EventHandler))
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

UserInterface::~UserInterface()
{
}

void UserInterface::keyPressEvent(Qt::Key key, Game *game)
{
    m_eventHandler->keyPressEvent(key, game);
}

void UserInterface::keyReleaseEvent(Qt::Key key, Game *game)
{
    m_eventHandler->keyReleaseEvent(key, game);
}

void UserInterface::mouseMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->mouseMoveEvent(x, y, game);
}

void UserInterface::touchMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->touchMoveEvent(x, y, game);
}

void UserInterface::gyroMoveEvent(qreal x, qreal y, Game *game)
{
    m_eventHandler->gyroMoveEvent(x, y, game);
}

void UserInterface::flickEvent(qreal startX, qreal x, Game *game)
{
    m_eventHandler->flickEvent(startX, x, game);
}

void UserInterface::flickReset(Game *game)
{
    m_eventHandler->flickReset(game);
}

void UserInterface::touchChargeFire(Game *game)
{
    m_eventHandler->touchChargeFire(game);
}

void UserInterface::touchFire(Game *game)
{
    m_eventHandler->touchFire(game);
}

void UserInterface::sync()
{
    //m_deferredActionHandler->processDeferredActions();

    //auto elapsedMilliseconds = m_timeStamp->restart();

    #ifdef Q_OS_MAC
        //m_scene->camera().setViewport(window()->width()*2, window()->height()*2);
    #else
        //m_scene->camera().setViewport(window()->width(), window()->height());
    #endif

    //m_scene->update(elapsedMilliseconds);
}

void UserInterface::render()
{
    //m_scene->render();
}

void UserInterface::cleanup()
{

}

void UserInterface::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeRendering()), this, SLOT(render()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);

        // force redraw
        //connect(m_timer.get(), &QTimer::timeout, win, &QQuickWindow::update);
        //m_timer->start(1000 / 60);
    }
}

}
