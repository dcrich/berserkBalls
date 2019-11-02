#include "geometrycreator.h"
#include <osg/Geode>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>



geometryCreator::geometryCreator()
{
}

void geometryCreator::makeMainSphere(osg::Sphere* sphere, osg::Geode* geode, float sphereRadius)
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

void geometryCreator::makeBoundaryBox(osg::Geode* mgeode, float cubeLength)
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

void geometryCreator::makeGoalBox(osg::Geode* mgeode, float cubeLength, std::vector<float> cornerPosition)
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
