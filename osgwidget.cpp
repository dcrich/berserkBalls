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

osg::Vec3 convert_physics_vector_to_vec3(std::vector<double> mVector)
{
    float mX = static_cast<float>(mVector[0]);
    float mY = static_cast<float>(mVector[1]);
    float mZ = static_cast<float>(mVector[2]);
    osg::Vec3 physicsVectorAsVec3(mX,mY,mZ);

    return physicsVectorAsVec3;
}

std::vector<int> check_for_collision(objectPosition *position, float boundary,float sphereRadius)
{
    std::vector<double> coordinates {position->get_position()};
    std::vector<int> checkCollision{0,0,0};
    double upperVirtualBoundary = static_cast<double>(boundary-sphereRadius);
    double lowerVirtualBoundary = static_cast<double>(sphereRadius);

    for (size_t i{0};i<coordinates.size();i++)
    {
        if (coordinates[i]<=lowerVirtualBoundary || coordinates[i]>=upperVirtualBoundary)
        {
            checkCollision[i] = 1;
            if(coordinates[i]<=lowerVirtualBoundary)
            {
                coordinates[i]=lowerVirtualBoundary;
            }
            else
            {
                coordinates[i] = upperVirtualBoundary;
            }
        }
        position->redefine_position(coordinates);
    }

    return checkCollision;
}

bool check_for_goal(objectPosition *position, float boundary, float goalSize)
{
    bool didYouWin{0};
    std::vector<double> coordinates {position->get_position()};
    double VirtualBoundary = static_cast<double>(boundary-goalSize);

    if (coordinates[0]>=VirtualBoundary && coordinates[1]>=VirtualBoundary && coordinates[2]>=VirtualBoundary)
    {
        didYouWin = 1;
        std::vector<double> zeroVelocity{0,0,0};
        position->update_velocity(zeroVelocity);
        std::vector<double> zeroDrag{0,0,0};
        position->update_drag(zeroDrag);
        std::vector<double> zeroAcceleration{0,0,0};
        position->update_acceleration(zeroAcceleration);
    }

    return didYouWin;
}
bool check_for_lose(objectPosition *position, bool didYouLose, float sphereRadius)
{
    std::vector<double> currentVelocity {position->get_velocity()};
    std::vector<double> coordinates {position->get_position()};
    double basicallyZero = .5;

    if (abs(currentVelocity[2])<=basicallyZero && coordinates[2]<=static_cast<double>(sphereRadius))
    {
        didYouLose = true;
    }
    else
    {
        didYouLose = false;
    }
    return didYouLose;
}

class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(objectPosition *spherePosition, float cubeLength, float sphereRadius)
    {
        mPosition = spherePosition;
        mBoundary = cubeLength;
        mRadius = sphereRadius;
    }

    ~SphereUpdateCallback()
    {
        delete mPosition;
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        std::vector<double> coordinates {mPosition->get_position()};
        std::vector<double> velocity {mPosition->get_velocity()};
        mPosition->update_position();
        coordinates  = mPosition->get_position();
        std::vector<int> collisionCheck = check_for_collision(mPosition,mBoundary,mRadius);

        for (size_t i{0};i<collisionCheck.size();i++)
        {
            if (collisionCheck[i])
            {
                mPosition->static_collision(i);
            }
        }
        mPosition->update_position();

        osg::Vec3d positionValue {convert_physics_vector_to_vec3(coordinates)};
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
        pat->setPosition(positionValue);

        traverse(node, nv);
    }
protected:
    objectPosition *mPosition;
    float mBoundary;
    float mRadius;
};


void makeMainSphere(osg::Sphere* sphere, osg::Geode* geode, float sphereRadius)
{
    osg::Vec3d physicalPositionV3d;
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

void makeBoundaryBox(osg::Geode* mgeode, float cubeLength)
{
    osg::Vec4 osgVec4color(1.f, 1.f, 1.f, 1.f);
    osg::Vec3 osgVec3dscaleFactor(1.f, 1.f, 1.f);
    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 8 );
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

void makeGoalBox(osg::Geode* mgeode, float cubeLength, std::vector<float> cornerPosition)
{
    osg::Vec4 osgVec4color(0.f, 1.f, 0.f, 1.f);
    osg::Vec3 osgVec3dscaleFactor(1.f, 1.f, 1.f);
    osg::Vec3Array* v = new osg::Vec3Array;
    float xCoord = cornerPosition[0];
    float yCoord = cornerPosition[1];
    float zCoord = cornerPosition[2];

    v->resize( 8 );
    (*v)[0].set( xCoord, yCoord, zCoord+cubeLength);
    (*v)[1].set(xCoord+cubeLength, yCoord, zCoord+cubeLength);
    (*v)[2].set(xCoord+cubeLength, yCoord+cubeLength, zCoord+cubeLength );
    (*v)[3].set(xCoord, yCoord+cubeLength, zCoord+cubeLength);
    (*v)[4].set(xCoord, yCoord, zCoord );
    (*v)[5].set(xCoord+cubeLength, yCoord, zCoord);
    (*v)[6].set(xCoord+cubeLength, yCoord+cubeLength, zCoord);
    (*v)[7].set(xCoord, yCoord+cubeLength, zCoord);

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
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops2 ) );
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

//void make_direction_line(osg::Geode* mgeode, std::vector<float> *lineEndPoint)
//{
//    osg::Vec4 osgVec4color(1.f, 1.f, 1.f, 1.f);
//    osg::Vec3 osgVec3dscaleFactor(1.f, 1.f, 1.f);
//    osg::Vec3Array* v = new osg::Vec3Array;
//    float x{*lineEndPoint[0]};
//            float y;
//            float z;
//    v->resize( 4 );
//    (*v)[0].set( 0.f, 0.f, 0.f);
//    (*v)[1].set(lineEndPoint[0], lineEndPoint[1], lineEndPoint[2]);

//    osg::Geometry* geom = new osg::Geometry;
//    geom->setUseDisplayList( false );
//    geom->setVertexArray( v );

//    osg::Vec4Array* c = new osg::Vec4Array;
//    c->push_back( osgVec4color );
//    geom->setColorArray( c, osg::Array::BIND_OVERALL );

//    GLushort idxLoops1[4] = {0, 1, 2, 3};
//    GLushort idxLoops2[4] = {4, 5, 6, 7};
//    GLushort idxLoops3[4] = {0, 1, 5, 4};
//    GLushort idxLoops4[4] = {3, 2, 6, 7};
//    GLushort idxLoops5[4] = {1, 2, 6, 5};
//    GLushort idxLoops6[4] = {0, 3, 7, 4};
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops1 ) );
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::POLYGON, 4, idxLoops2 ) );
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops3 ) );
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops4 ) );
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops5 ) );
//    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops6 ) );


//    mgeode->addDrawable( geom );

//    mgeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
//    mgeode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
//    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
//    transform->setScale(osgVec3dscaleFactor);

//    transform->addChild(mgeode);
//    osg::StateSet* mstateSet = mgeode->getOrCreateStateSet();
//    osg::Material* mmaterial = new osg::Material;

//    mmaterial->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );

//    mstateSet->setAttributeAndModes( mmaterial, osg::StateAttribute::ON );
//    mstateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
//}

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

    //initialize timer
    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};

    //create line
    osg::Geode* lGeode = new osg::Geode;

    //creates box
    osg::Geode* mgeode = new osg::Geode;
    makeBoundaryBox(mgeode,cubeLength);
    mRoot->addChild(mgeode);

    //creates sphere
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
    makeMainSphere(sphere, geode, sphereRadius);
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setPosition(spherePositionVec3);
    transform->setUpdateCallback(new SphereUpdateCallback(loadedSphere, cubeLength, sphereRadius));
    transform->addChild(geode);
    mRoot->addChild(transform);

    //create goal box
    osg::Geode* gGeode = new osg::Geode;
    std::vector<float> goalPosition{cubeLength-goalSize,cubeLength-goalSize,cubeLength-goalSize};
    makeGoalBox(gGeode,goalSize,goalPosition);
    mRoot->addChild(gGeode);




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
    didYouLose = false;
    backgroundColor = {0.25f, 0.1f, 0.4f, 1.f };
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
    backgroundColor = {0.25f, 0.1f, 0.4f, 1.f };
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
        backgroundColor = {0.f, 0.5f, 0.f, 1.f };
        camera->setClearColor( backgroundColor );
        didYouLose = false;
    }
    if (didYouLose == true && hasGameStarted == true)
    {
        backgroundColor = {0.5f, 0.0f, 0.f, 1.f };
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


