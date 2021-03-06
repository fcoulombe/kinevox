/*
* Copyright (C) 2011 by Francois Coulombe
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#ifdef ENABLE_COLLADA
#include "common/ColladaManager.h"
#include <gcl/Assert.h>
#include <gcl/Path.h>

#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

using namespace GCL;
class BaseWriter;

class KinevoxWriter : public COLLADAFW::IWriter
{
public:
    enum Runs
    {
        SCENEGRAPH_RUN,
        GEOMETRY_RUN
    };

    typedef std::list<COLLADAFW::LibraryNodes> LibraryNodesList;

    struct InstanceGeometryInfo
    {
        InstanceGeometryInfo( COLLADAFW::InstanceGeometry* _fwInstanceGeometry, const COLLADABU::Math::Matrix4& _worldMatrix)
            :  fwInstanceGeometry(_fwInstanceGeometry), worldMatrix(_worldMatrix) {}
        COLLADAFW::InstanceGeometry* fwInstanceGeometry;
        COLLADABU::Math::Matrix4 worldMatrix;
    };

    typedef std::multimap<COLLADAFW::UniqueId, InstanceGeometryInfo> GeometryUniqueIdInstanceGeometryInfoMap;

    /** Maps unique ids of framework materials to the corresponding framework material.*/
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Material> UniqueIdFWMaterialMap;

    /** Maps unique ids of framework images to the corresponding framework image.*/
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Image> UniqueIdFWImageMap;

    /** Maps unique ids of framework effects to the corresponding framework material.*/
    typedef std::map<COLLADAFW::UniqueId, COLLADAFW::Effect> UniqueIdFWEffectMap;

    enum Severity
    {
        SEVERITY_INFORMATION,
        SEVERITY_WARNING,
        _SEVERITY_ERROR
    };

private:
    COLLADABU::URI mInputFile;
    COLLADABU::URI mOutputFile;
    Runs mCurrentRun;

    const COLLADAFW::VisualScene* mVisualScene;
    LibraryNodesList mLibrayNodesList;

    GeometryUniqueIdInstanceGeometryInfoMap mGeometryUniqueIdInstanceGeometryInfoMap;

    UniqueIdFWMaterialMap mUniqueIdFWMaterialMap;
    UniqueIdFWImageMap mUniqueIdFWImageMap;
    UniqueIdFWEffectMap mUniqueIdFWEffectMap;

public:
    KinevoxWriter( );
    virtual ~KinevoxWriter();

public:
    static void reportError(const std::string & method, const std::string & message, Severity severity);

public:

    bool write(const COLLADABU::URI& inputFile);

    /** Deletes the entire scene.
    @param errorMessage A message containing informations about the error that occurred.
    */
    void cancel(const std::string & errorMessage);

    /** Prepare to receive data.*/
    void start();

    /** Remove all objects that don't have an object. Deletes unused visual scenes.*/
    void finish();

    /** When this method is called, the writer must write the global document asset.
    @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );

    /** Writes the entire visual scene.
    @return True on succeeded, false otherwise.*/
    virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );

    /** Writes the scene.
    @return True on succeeded, false otherwise.*/
    virtual bool writeScene ( const COLLADAFW::Scene* scene );

    /** Handles all nodes in the library nodes.
    @return True on succeeded, false otherwise.*/
    virtual bool writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes );

    /** Writes the geometry.
    @return True on succeeded, false otherwise.*/
    virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );

    /** Writes the material.
    @return True on succeeded, false otherwise.*/
    virtual bool writeMaterial( const COLLADAFW::Material* material );

    /** Writes the effect.
    @return True on succeeded, false otherwise.*/
    virtual bool writeEffect( const COLLADAFW::Effect* effect );

    /** Writes the camera.
    @return True on succeeded, false otherwise.*/
    virtual bool writeCamera( const COLLADAFW::Camera* camera );

    /** Writes the image.
    @return True on succeeded, false otherwise.*/
    virtual bool writeImage( const COLLADAFW::Image* image );

    /** Writes the light.
    @return True on succeeded, false otherwise.*/
    virtual bool writeLight( const COLLADAFW::Light* light );

    /** Writes the animation.
    @return True on succeeded, false otherwise.*/
    virtual bool writeAnimation( const COLLADAFW::Animation* animation );

    /** Writes the animation.
    @return True on succeeded, false otherwise.*/
    virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );

    /** Writes the skin controller data.
    @return True on succeeded, false otherwise.*/
    virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );

    /** Writes the controller.
    @return True on succeeded, false otherwise.*/
    virtual bool writeController( const COLLADAFW::Controller* Controller );

    /** When this method is called, the writer must write the formulas. All the formulas of the entire
    COLLADA file are contained in @a formulas.
    @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeFormulas( const COLLADAFW::Formulas* /*formulas*/ ){return true;}

    /** When this method is called, the writer must write the kinematics scene. 
    @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* /*kinematicsScene*/ ){return true;};



public:
    friend class BaseWriter;

    GeometryUniqueIdInstanceGeometryInfoMap& getGeometryUniqueIdInstanceGeometryInfoMap() { return mGeometryUniqueIdInstanceGeometryInfoMap; }

    UniqueIdFWMaterialMap& getUniqueIdFWMaterialMap() { return mUniqueIdFWMaterialMap; }

    UniqueIdFWImageMap& getUniqueIdFWImageMap() { return mUniqueIdFWImageMap; }

    UniqueIdFWEffectMap& getUniqueIdFWEffectMap() { return mUniqueIdFWEffectMap; }

    const COLLADABU::URI& getInputFile() const { return mInputFile; }

    const COLLADABU::URI& getOutputFile() const { return mOutputFile; }


private:
    /** Disable default copy ctor. */
    KinevoxWriter( const KinevoxWriter& pre );
    /** Disable default assignment operator. */
    const KinevoxWriter& operator= ( const KinevoxWriter& pre );

};


void ColladaManager::Initialize()
{

}


void ColladaManager::Terminate()
{
}

static ToolMeshData meshData;

bool ColladaManager::LoadScene(const char* pFilename)
{
    GCLAssertMsg(Path::GetFileNameExtension(pFilename) == "dae", pFilename);
    KinevoxWriter writer;
    writer.write(COLLADABU::URI::nativePathToUri(pFilename));
    return true;
}


ToolMeshData &ColladaManager::GetMeshData()
{
    return meshData;
}

static ToolNodeData nodeData;

ToolNodeData &ColladaManager::GetNodeData()
{
    return nodeData;
}


//--------------------------------------------------------------------
KinevoxWriter::KinevoxWriter( )
    :  mCurrentRun(SCENEGRAPH_RUN)
{
    mVisualScene = NULL;
}

//--------------------------------------------------------------------
KinevoxWriter::~KinevoxWriter()
{
    delete mVisualScene;
}

//--------------------------------------------------------------------
void KinevoxWriter::reportError( const std::string& method, const std::string& message, Severity severity )
{
    switch ( severity )
    {
    case SEVERITY_INFORMATION:
        std::cout << "Information"; 
        break;
    case SEVERITY_WARNING:
        std::cerr << "Warning"; 
        break;
    case _SEVERITY_ERROR:
        std::cerr << "Error"; 
        break;
    }
    if ( !method.empty() )
        std::cout << " in " << method;
    std::cout << ": ";
    std::cout << message << std::endl;
}


void ParseNodeHierarchy(const COLLADAFW::Node * node, ToolNodeData &parent)
{
    parent.mName = node->getName().c_str();
    COLLADABU::Math::Matrix4 transform = node->getTransformationMatrix();
    parent.mTransform[0].x = transform[0][0];
    parent.mTransform[0].y = transform[0][1];
    parent.mTransform[0].z = transform[0][2];
    parent.mTransform[0].w = transform[0][3];

    parent.mTransform[1].x = transform[1][0];
    parent.mTransform[1].y = transform[1][1];
    parent.mTransform[1].z = transform[1][2];
    parent.mTransform[1].w = transform[1][3];

    parent.mTransform[2].x = transform[2][0];
    parent.mTransform[2].y = transform[2][1];
    parent.mTransform[2].z = transform[2][2];
    parent.mTransform[2].w = transform[2][3];

    parent.mTransform[3].x = transform[3][0];
    parent.mTransform[3].y = transform[3][1];
    parent.mTransform[3].z = transform[3][2];
    parent.mTransform[3].w = transform[3][3];

    const COLLADAFW::NodePointerArray& childs = node->getChildNodes();
    for (size_t i=0; i<childs.getCount(); ++i)
    {
        parent.mNodeList.push_back(ToolNodeData());
        ToolNodeData &newNode = parent.mNodeList.back();
        const COLLADAFW::Node *tempNode = childs[i];
        ParseNodeHierarchy(tempNode, newNode);
    }
}
//--------------------------------------------------------------------
bool KinevoxWriter::write(const COLLADABU::URI& inputFile)
{
    mInputFile = inputFile;
    COLLADASaxFWL::Loader loader;
    COLLADAFW::Root root(&loader, this);

    // Load scene graph 
    if ( !root.loadDocument(mInputFile.toNativePath()) )
        return false;

    for (UniqueIdFWMaterialMap::iterator it = mUniqueIdFWMaterialMap.begin(); it != mUniqueIdFWMaterialMap.end(); ++it)
    {
        meshData.mMaterialCount++;
        COLLADAFW::Material &mat = it->second;
       // std::cout << "mat name: " << mat.getName().c_str() << std::endl;
        UniqueIdFWEffectMap::iterator res = mUniqueIdFWEffectMap.find(mat.getInstantiatedEffect());
        GCLAssertMsg(res != mUniqueIdFWEffectMap.end(), mat.getInstantiatedEffect().toAscii().c_str());
        
        COLLADAFW::Effect &fx = res->second;
        const COLLADAFW::CommonEffectPointerArray& commonFx = fx.getCommonEffects();
        for (size_t i=0; i<commonFx.getCount(); ++i)
        {
            ToolMaterialData &matData = meshData.mMaterialData;
            
            matData.matName = mat.getName().c_str();
           
            const COLLADAFW::EffectCommon *tempCommonFx = commonFx[i];
            const COLLADAFW::ColorOrTexture &ambient = tempCommonFx->getAmbient();
            GCLAssert(ambient.getType() == COLLADAFW::ColorOrTexture::COLOR);
            COLLADAFW::Color a = ambient.getColor();
            matData.mAmbient.x = a.getRed();
            matData.mAmbient.y = a.getGreen();
            matData.mAmbient.z = a.getBlue();
            matData.mAmbient.w = a.getAlpha();

            const COLLADAFW::ColorOrTexture &diffuse = tempCommonFx->getDiffuse();
            if (diffuse.getType() != COLLADAFW::ColorOrTexture::TEXTURE)
                continue;
            const COLLADAFW::Texture &tex = diffuse.getTexture();
           //std::cout << tempCommonFx->getSamplerPointerArray()[tex.getSamplerId()]->getSourceImage().toAscii().c_str() << std::endl;
             UniqueIdFWImageMap::iterator texIt = mUniqueIdFWImageMap.find(tempCommonFx->getSamplerPointerArray()[tex.getSamplerId()]->getSourceImage());
            GCLAssert(texIt != mUniqueIdFWImageMap.end());
            COLLADAFW::Image &img = texIt->second;
            //std::cout << img.getImageURI().getPathFile().c_str() << std::endl;
            matData.texName = img.getImageURI().getPathFile().c_str() ;
            
            const COLLADAFW::ColorOrTexture &emission = tempCommonFx->getEmission();
            GCLAssert(emission.getType() == COLLADAFW::ColorOrTexture::COLOR);
            matData.mEmissive.x = emission.getColor().getRed();
            matData.mEmissive.y = emission.getColor().getGreen();
            matData.mEmissive.z = emission.getColor().getBlue();
            matData.mEmissive.w = emission.getColor().getAlpha();

            const COLLADAFW::FloatOrParam &shininess = tempCommonFx->getShininess();
            GCLAssert(shininess.getType() == COLLADAFW::FloatOrParam::FLOAT);
            matData.mShininess = shininess.getFloatValue();

            //const COLLADAFW::ColorOrTexture &spec = tempCommonFx->getSpecular();
            //GCLAssert(spec.getType() == COLLADAFW::ColorOrTexture::COLOR);
             
        }


    }

    // if there is no visual scene in the COLLADA file, nothing to export here
    if ( mVisualScene )
    {
        const COLLADAFW::NodePointerArray& nodesToWrite = mVisualScene->getRootNodes();
        GCLAssert(nodesToWrite.getCount() == 1);
        ParseNodeHierarchy(nodesToWrite[0], nodeData);
    }

    // load and write geometries
    mCurrentRun = GEOMETRY_RUN;
    if ( !root.loadDocument(mInputFile.toNativePath()) )
        return false;

    return true;
}

//--------------------------------------------------------------------
void KinevoxWriter::cancel( const std::string& /*errorMessage*/ )
{
}

//--------------------------------------------------------------------
void KinevoxWriter::start()
{
}

//--------------------------------------------------------------------
void KinevoxWriter::finish()
{
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeGlobalAsset( const COLLADAFW::FileInfo* /*asset*/ )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeVisualScene( const COLLADAFW::VisualScene* visualScene )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;


    // todo. handle multiple occurences of visual scene properly. This is just a quick fix
    if ( mVisualScene )
    {
        delete mVisualScene;
    }
    mVisualScene = new COLLADAFW::VisualScene(*visualScene);
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeScene( const COLLADAFW::Scene* /*scene*/ )
{
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeLibraryNodes( const COLLADAFW::LibraryNodes* libraryNodes )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    mLibrayNodesList.push_back(*libraryNodes);
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeGeometry( const COLLADAFW::Geometry* geometry )
{
    if ( mCurrentRun != GEOMETRY_RUN )
        return true;
    if ( geometry->getType() != COLLADAFW::Geometry::GEO_TYPE_MESH )
        return true;

   
    meshData.mSubMeshList.clear();
     COLLADAFW::Mesh *mesh = (COLLADAFW::Mesh *)(geometry); //const cast because collada doesnt respect const correctness
#if 0
    std::cout << geometry->getName().c_str() << std::endl;
   
    std::cout << "polycnt: " << mesh->getPolygonsCount()<<std::endl;
    std::cout << "polygonpolycnt: " << mesh->getPolygonsPolygonCount()<<std::endl;

    std::cout << "poly vertx: " << mesh->getPositions().getValuesCount()<<std::endl;
    std::cout << "normal: " << mesh->getNormalsCount()<<std::endl;
    std::cout << "uv: " << mesh->getUVCoords().getValuesCount()<<std::endl;
    std::cout << "vtc: " << mesh->getColors().getValuesCount()<<std::endl;

    std::cout << "primi: " << mesh->getPrimitiveCount(COLLADAFW::MeshPrimitive::TRIANGLES)<<std::endl;
    std::cout << "meshprimi: " << mesh->getMeshPrimitiveCount(COLLADAFW::MeshPrimitive::TRIANGLES)<<std::endl;
#endif
    const COLLADAFW::MeshPrimitiveArray& subMeshes = mesh->getMeshPrimitives();
    size_t subMeshCount = subMeshes.getCount();
    meshData.mSubMeshList.resize(subMeshCount);

    const COLLADAFW::MeshVertexData& normals = mesh->getNormals();
    const COLLADAFW::MeshVertexData& positions = mesh->getPositions();
    const COLLADAFW::MeshVertexData& uvs = mesh->getUVCoords();
    //const COLLADAFW::MeshVertexData& colors = mesh->getColors();

    for (size_t i=0; i<subMeshes.getCount(); ++i)
    {
        ToolSubMeshData &subMeshData = meshData.mSubMeshList[i];
        const COLLADAFW::MeshPrimitive* subMesh = subMeshes[i];
        const COLLADAFW::UIntValuesArray& posi = subMesh->getPositionIndices();
        const COLLADAFW::UIntValuesArray& normi = subMesh->getNormalIndices();
        const COLLADAFW::UIntValuesArray& uvi = subMesh->getUVCoordIndices(0)->getIndices();
        //const COLLADAFW::UIntValuesArray& coli = subMesh->getColorIndices(0)->getIndices();
        std::cout << "indice count: " << posi.getCount()<<std::endl;
        switch (subMesh->getPrimitiveType())
        {
        case COLLADAFW::MeshPrimitive::TRIANGLES:
            for (size_t j=0; j<posi.getCount(); ++j)
            {
                Point3<MeshReal> newPosition;
                if (positions.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
                {
                    const double *rawPos = positions.getDoubleValues()->getData();
                    int positionIndex = posi[j]*3;           
                    newPosition.x = (MeshReal)(rawPos[positionIndex]);
                    newPosition.y = (MeshReal)(rawPos[positionIndex+2]);
                    newPosition.z = (MeshReal)(rawPos[positionIndex+1]);
                    subMeshData.mVertexList.push_back(newPosition);
                }
                else
                {
                    const float *rawPos = positions.getFloatValues()->getData();
                    int positionIndex = posi[j]*3;           
                    newPosition.x = rawPos[positionIndex];
                    newPosition.y = rawPos[positionIndex+2];
                    newPosition.z = rawPos[positionIndex+1];
                    subMeshData.mVertexList.push_back(newPosition);
                }
                //std::cout << "v" << j << ": " << newPosition << std::endl;
                Point3<MeshReal> newNormal;
                if (normals.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
                {
                    const double *rawNorm = normals.getDoubleValues()->getData();
                    int normalIndex = normi[j]*3;           
                    newNormal.x = (MeshReal)(rawNorm[normalIndex]);
                    newNormal.y = (MeshReal)(rawNorm[normalIndex+2]);
                    newNormal.z = (MeshReal)(rawNorm[normalIndex+1]);
                    newNormal.Normalize();
                    subMeshData.mNormalList.push_back(newNormal);
                }
                else
                {
                    const float *rawNorm = positions.getFloatValues()->getData();
                    int normalIndex = normi[j]*3;           
                    newNormal.x = rawNorm[normalIndex];
                    newNormal.y = rawNorm[normalIndex+2];
                    newNormal.z = rawNorm[normalIndex+1];
                    newNormal.Normalize();
                    subMeshData.mNormalList.push_back(newNormal);
                }

                Point2<MeshReal> newUv;
                if (uvs.getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE )
                {
                    const double *rawUv = uvs.getDoubleValues()->getData();
                    size_t uvIndex = uvi[j]*uvs.getStride(0);           
                    newUv.x = (MeshReal)(rawUv[uvIndex]);
                    newUv.y = (MeshReal)(rawUv[uvIndex+1]);
                    subMeshData.mUvList.push_back(newUv);
                }
                else
                {
                    const float *rawUv = uvs.getFloatValues()->getData();
                    size_t uvIndex = uvi[j]*uvs.getStride(0);     
                    newUv.x = rawUv[uvIndex];
                    newUv.y = rawUv[uvIndex+1];
                    subMeshData.mUvList.push_back(newUv);
                }
                subMeshData.mIndiceList.push_back((int)j);

            }
            break;
        default:
            GCLAssertMsg(false, "only support triangle primitives");
        }
    }

    //MeshWriter meshWriter( this, (COLLADAFW::Mesh*)geometry );
    return true; //meshWriter.write();
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeMaterial( const COLLADAFW::Material* material )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    mUniqueIdFWMaterialMap.insert(std::make_pair(material->getUniqueId(),*material ));
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeEffect( const COLLADAFW::Effect* effect )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    mUniqueIdFWEffectMap.insert(std::make_pair(effect->getUniqueId(),*effect ));
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeCamera( const COLLADAFW::Camera* /*camera*/ )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeImage( const COLLADAFW::Image* image )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    mUniqueIdFWImageMap.insert(std::make_pair(image->getUniqueId(),*image ));
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeLight( const COLLADAFW::Light* /*light*/ )
{
    if ( mCurrentRun != SCENEGRAPH_RUN )
        return true;
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeAnimation( const COLLADAFW::Animation* /*animation*/ )
{
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeAnimationList( const COLLADAFW::AnimationList* /*animationList*/ )
{
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeSkinControllerData( const COLLADAFW::SkinControllerData* /*skinControllerData*/ )
{
    return true;
}

//--------------------------------------------------------------------
bool KinevoxWriter::writeController( const COLLADAFW::Controller* /*Controller*/ )
{
    return true;
}
#endif
