#include "OSGWidget.h"
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

osg::Vec3 convert_physics_vector_to_vec3(std::vector<double> mVector)
{
    float mX = static_cast<float>(mVector[0]);
    float mY = static_cast<float>(mVector[1]);
    float mZ = static_cast<float>(mVector[2]);
    osg::Vec3 physicsVectorAsVec3(mX,mY,mZ);

    return physicsVectorAsVec3;
}

std::vector<int> check_for_collision(objectPosition *position)
{
    std::vector<double> coordinates {position->get_position()};
    std::vector<int> checkCollision{0,0,0};

    for (size_t i{0};i<coordinates.size();i++)
    {
        if (coordinates[i]<=0 || coordinates[i]>=195)
        {
            checkCollision[i] = 1;
        }
    }

    return checkCollision;
}

class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(objectPosition *spherePosition)
    {
        position = spherePosition;
    }

    ~SphereUpdateCallback()
    {
        delete position;
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        std::vector<double> coordinates {position->get_position()};
        std::vector<double> velocity {position->get_velocity()};
        std::vector<double> coordinatesbottom {0,0,6};

        position->update_position();
        coordinates  = position->get_position();
        std::vector<int> collisionCheck = check_for_collision(position);

        for (size_t i{0};i<collisionCheck.size();i++)
        {
            if (collisionCheck[i])
            {
                position->static_collision(i);
                position->redefine_position(coordinates);
            }
        }
        position->update_position();

        osg::Vec3d positionValue {convert_physics_vector_to_vec3(coordinates)};
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
        pat->setPosition(positionValue);

        traverse(node, nv);

    }
protected:
    objectPosition *position;
};


void makeMainSphere(osg::Sphere* sphere, osg::Geode* geode )
{
    osg::Vec3d physicalPositionV3d;
    float sphereRadius = 8.f;
    sphere->set(physicalPositionV3d,sphereRadius);
    osg::ShapeDrawable* sd = new osg::ShapeDrawable( sphere );
    sd->setColor( osg::Vec4( 1.f, 1.f, 1.f, 1.f ) );
    sd->setName( "Sphere" );
    geode->addDrawable( sd );
    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
}

void makeBoundaryBox(osg::Geode* mgeode)
{
    osg::Vec4 osgVec4color(1.f, 1.f, 1.f, 1.f);
    osg::Vec3 osgVec3dscaleFactor(1.f, 1.f, 1.f);
    osg::Vec3Array* v = new osg::Vec3Array;
    float cubeLength{200.};
    v->resize( 4 );
    (*v)[0].set( 0.f, 0.f, cubeLength);
    (*v)[1].set(cubeLength, 0.f, cubeLength);
    (*v)[2].set(cubeLength, cubeLength, cubeLength );
    (*v)[3].set(0.f, cubeLength, cubeLength);
    (*v)[4].set(0.f, 0.f, 0.f );
    (*v)[5].set(cubeLength, 0.f, 0.f );
    (*v)[6].set(cubeLength, cubeLength, 0.f );
    (*v)[7].set(0.f, cubeLength, 0.f);

    osg::Geometry* geom = new osg::Geometry;
    geom->setUseDisplayList( false );
    geom->setVertexArray( v );

    osg::Vec4Array* c = new osg::Vec4Array;
    c->push_back( osgVec4color );
    geom->setColorArray( c, osg::Array::BIND_OVERALL );

    GLushort idxLoops1[4] = {0, 1, 2, 3};
    GLushort idxLoops2[4] = {4, 5, 6, 7};
    GLushort idxLoops3[4] = {0, 1, 5, 4};
    GLushort idxLoops4[4] = {3, 2, 6, 7};
    GLushort idxLoops5[4] = {1, 2, 6, 5};
    GLushort idxLoops6[4] = {0, 3, 7, 4};
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops1 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::POLYGON, 4, idxLoops2 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops3 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops4 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops5 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops6 ) );


    mgeode->addDrawable( geom );

    mgeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    mgeode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    transform->setScale(osgVec3dscaleFactor);

    transform->addChild(mgeode);
    osg::StateSet* mstateSet = mgeode->getOrCreateStateSet();
    osg::Material* mmaterial = new osg::Material;

    mmaterial->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );

    mstateSet->setAttributeAndModes( mmaterial, osg::StateAttribute::ON );
    mstateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
}

OSGWidget::OSGWidget( QWidget* parent, Qt::WindowFlags flags ):
    QOpenGLWidget{ parent,flags },
    mGraphicsWindow{ new osgViewer::GraphicsWindowEmbedded{ this->x(),
                                                            this->y(),
                                                            this->width(),
                                                            this->height() } }
  , mViewer{ new osgViewer::CompositeViewer }
{
    mRoot = new osg::Group;
    // create camera and view how to get the ball to be the center of the camera?
    osgViewer::View* mView = new osgViewer::View;
    float aspectRatio = static_cast<float>( this->width() ) / static_cast<float>( this->height() );
    auto pixelRatio   = this->devicePixelRatio();
    osg::Camera* camera = new osg::Camera;
    camera->setViewport( 0, 0, this->width() * pixelRatio, this->height() * pixelRatio );
    camera->setClearColor( osg::Vec4( 0.25f, 0.1f, 0.4f, 1.f ) );
    camera->setProjectionMatrixAsPerspective( 45.f, aspectRatio, 1.f, 1000.f );
    camera->setGraphicsContext( mGraphicsWindow );
    mView->setCamera( camera );
    mView->setSceneData( mRoot.get() );
    mView->addEventHandler( new osgViewer::StatsHandler );
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow( false );
    manipulator->setHomePosition(osg::Vec3d(200.0,-500.0,250.0),osg::Vec3d(200,500,-10),osg::Vec3d(0,0,1));
    mView->setCameraManipulator( manipulator );
    mViewer->addView( mView );
    mViewer->setThreadingModel( osgViewer::CompositeViewer::SingleThreaded );
    mViewer->realize();
    mView->home();

    //initialize timer
    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};

    //creates sphere
    loadedSphere = new objectPosition;
    loadedSphere->update_timeStep(timeStep);
    std::vector<double> startPosition{0,0,0};
    loadedSphere->redefine_position(startPosition);
    std::vector<double> startVelocity{0,0,0};
    loadedSphere->update_velocity(startVelocity);
    std::vector<double> startAcceleration{0,0,0};
    loadedSphere->update_acceleration(startAcceleration);
    osg::Vec3 spherePositionVec3 = convert_physics_vector_to_vec3(loadedSphere->get_position());
    osg::Sphere* sphere = new osg::Sphere(spherePositionVec3, 1.0f );
    osg::Geode* geode = new osg::Geode;
    makeMainSphere(sphere, geode);
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setPosition(spherePositionVec3);
    transform->setUpdateCallback(new SphereUpdateCallback(loadedSphere));
    transform->addChild(geode);
    mRoot->addChild(transform);
    //creates box
    osg::Geode* mgeode = new osg::Geode;
    makeBoundaryBox(mgeode);
    mRoot->addChild(mgeode);



    //idk what this does
    this->setFocusPolicy( Qt::StrongFocus );
    this->setMinimumSize(100, 100 );
    this->setMouseTracking( true );
    this->update();

    // define length of timer, start timer
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId=startTimer(timerDurationInMilliSeconds);

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

//loaded sphere update physics values
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    update();
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


