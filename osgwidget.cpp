#include "osgwidget.h"
#include <osg/Camera>
#include <osg/DisplaySettings>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osgDB/WriteFile>
#include <osgGA/EventQueue>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/NodeVisitor>
#include <osg/LineWidth>
#include <osgUtil/SmoothingVisitor>
#include <osgParticle/FireEffect>
#include <cassert>
#include <vector>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include "objectposition.h"
#include "sphereupdatecallback.h"
#include "stdtoosgadapt.h"
#include "geometrycreator.h"
#include "worldcollisions.h"


OSGWidget::OSGWidget( QWidget* parent, Qt::WindowFlags flags ):
    QOpenGLWidget{ parent,flags },
    mGraphicsWindow{ new osgViewer::GraphicsWindowEmbedded{ this->x(),
                     this->y(),
                     this->width(),
                     this->height() } }
  , mViewer{ new osgViewer::CompositeViewer }
{
    mRoot = new osg::Group;
    // create camera and view
    osgViewer::View* mView = new osgViewer::View;
    double aspectRatio = static_cast<double>( this->width() ) / static_cast<double>( this->height() );
    auto pixelRatio   = this->devicePixelRatio();
    camera = new osg::Camera;
    camera->setViewport( 0, 0, this->width() * pixelRatio, this->height() * pixelRatio );
    camera->setClearColor( backgroundColor );
    camera->setProjectionMatrixAsPerspective( 45.0, aspectRatio, 1.0, 1000.0);
    camera->setGraphicsContext( mGraphicsWindow );
    mView->setCamera( camera );
    mView->setSceneData( mRoot.get() );
    mView->addEventHandler( new osgViewer::StatsHandler );
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow( false );
    manipulator->setHomePosition(osg::Vec3d(-25.0,-250.0,50.0),osg::Vec3d(70,0,100),osg::Vec3d(0,0,1));
    mView->setCameraManipulator( manipulator );
    mViewer->addView( mView );
    mViewer->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
    mViewer->realize();
    mView->home();

    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};

    geometryCreator geometryWorld;

    osg::Geode* mgeode = new osg::Geode;
    geometryWorld.makeBoundaryBox(mgeode,cubeLength);
    mRoot->addChild(mgeode);

    loadedSphere = new objectPosition;
    loadedSphere->update_timeStep(timeStep);
    std::vector<double> startPosition{0,0,static_cast<double>(sphereRadius)};
    loadedSphere->redefine_position(startPosition);
    std::vector<double> startVelocity{0,0,0};
    loadedSphere->update_velocity(startVelocity);
    std::vector<double> startAcceleration{0,0,0};
    loadedSphere->update_acceleration(startAcceleration);
    osg::Vec3 spherePositionVec3 = convert_physics_vector_to_vec3(loadedSphere->get_position());
    osg::Sphere* sphere = new osg::Sphere(spherePositionVec3,sphereRadius);
    osg::Geode* geode = new osg::Geode;
    geometryWorld.makeMainSphere(sphere, geode, sphereRadius);
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setPosition(spherePositionVec3);
    transform->setUpdateCallback(new sphereUpdateCallback(loadedSphere, cubeLength, sphereRadius));
    transform->addChild(geode);
    mRoot->addChild(transform);

    osg::Geode* gGeode = new osg::Geode;
    std::vector<float> goalPosition{cubeLength-goalSize,cubeLength-goalSize,cubeLength-goalSize};
    geometryWorld.makeGoalBox(gGeode,goalSize,goalPosition);
    mRoot->addChild(gGeode);

    this->setFocusPolicy( Qt::StrongFocus );
    this->setMinimumSize(100, 100 );
    this->setMouseTracking( true );
    this->update();

    // define length of timer, start timer
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId=startTimer(static_cast<int>(timerDurationInMilliSeconds));

}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}

void OSGWidget::Fire(std::vector<double> newVelocity)
{
    std::vector<double> gravityVector{0,0,-9.8};
    loadedSphere->update_acceleration(gravityVector);
    loadedSphere->update_velocity(newVelocity);
    didYouLose = false;
    backgroundColor = {0.1f, 0.1f, 0.1f, 1.f };
    camera->setClearColor( backgroundColor );
    hasGameStarted = true;
}
void OSGWidget::reset_game()
{
    std::vector<double> startPosition{0,0,0};
    loadedSphere->redefine_position(startPosition);
    std::vector<double> startVelocity{0,0,0};
    loadedSphere->update_velocity(startVelocity);
    std::vector<double> startAcceleration{0,0,0};
    loadedSphere->update_acceleration(startAcceleration);
    backgroundColor = {0.1f, 0.1f, 0.1f, 1.f };
    camera->setClearColor( backgroundColor );
    hasGameStarted = false;
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    update();
    bool didYouWin = check_for_goal(loadedSphere,cubeLength,goalSize);
    didYouLose = check_for_lose(loadedSphere,didYouLose, sphereRadius);
    if (didYouWin)
    {
        backgroundColor = {0.f, 0.3f, 0.f, 1.f };
        camera->setClearColor( backgroundColor );
        didYouLose = false;
    }
    if (didYouLose == true && hasGameStarted == true)
    {
        backgroundColor = {0.4f, 0.0f, 0.f, 1.f };
        camera->setClearColor( backgroundColor );

    }
}

void OSGWidget::paintEvent( QPaintEvent* /* paintEvent */ )
{
    this->makeCurrent();

    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );

    this->paintGL();

    painter.end();

    this->doneCurrent();
}

void OSGWidget::paintGL()
{
    mViewer->frame();
}

void OSGWidget::resizeGL( int width, int height )
{
    this->getEventQueue()->windowResize( this->x(), this->y(), width, height );
    mGraphicsWindow->resized( this->x(), this->y(), width, height );

    this->on_resize( width, height );
}

void OSGWidget::keyPressEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    if( event->key() == Qt::Key_H )
    {
        mView->home();
        return;
    }

    this->getEventQueue()->keyPress( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}

void OSGWidget::keyReleaseEvent( QKeyEvent* event )
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    this->getEventQueue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol( *keyData ) );
}


void OSGWidget::mouseMoveEvent( QMouseEvent* event )
{

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseMotion( static_cast<float>( event->x() * pixelRatio ),
                                        static_cast<float>( event->y() * pixelRatio ) );
}

void OSGWidget::mousePressEvent( QMouseEvent* event )
{
    // 1 = left mouse button
    // 2 = middle mouse button
    // 3 = right mouse button

    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonPress( static_cast<float>( event->x() * pixelRatio ),
                                             static_cast<float>( event->y() * pixelRatio ),
                                             button );

}

void OSGWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // 1 = left mouse button
    // 2 = middle mouse button
    // 3 = right mouse button

    unsigned int button = 0;

    switch( event->button() )
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    default:
        break;
    }

    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonRelease( static_cast<float>( pixelRatio * event->x() ),
                                               static_cast<float>( pixelRatio * event->y() ),
                                               button );
}

void OSGWidget::wheelEvent( QWheelEvent* event )
{
    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                                 : osgGA::GUIEventAdapter::SCROLL_DOWN;

    this->getEventQueue()->mouseScroll( motion );
}

void OSGWidget::on_resize( int width, int height )
{
    std::vector<osg::Camera*> cameras;
    mViewer->getCameras( cameras );

    auto pixelRatio = this->devicePixelRatio();

    cameras[0]->setViewport( 0, 0, width * pixelRatio, height * pixelRatio );
}

osgGA::EventQueue* OSGWidget::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = mGraphicsWindow->getEventQueue();

    if( eventQueue )
        return eventQueue;
    else
        throw std::runtime_error( "Unable to obtain valid event queue");
}

bool OSGWidget::event( QEvent* event )
{
    bool handled = QOpenGLWidget::event( event );

    repaint_osg_graphics_after_interaction(event);

    return handled;
}

void OSGWidget::repaint_osg_graphics_after_interaction(QEvent* event)
{
    switch( event->type() )
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
        break;

    default:
        break;
    }
}


