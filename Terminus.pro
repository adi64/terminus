TEMPLATE = app

CONFIG += c++11

QT += qml quick widgets multimedia sensors

RESOURCES += qml.qrc \
    shaders.qrc \
    materials.qrc \
    models.qrc \
    sounds.qrc \
    textures.qrc

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    sounds/engine/apcfly.wav \
    sounds/engine/apcfly2.wav \
    sounds/engine/bee.wav \
    sounds/engine/Bipedm.wav \
    sounds/engine/hoverl.wav \
    sounds/engine/weapchng.wav \
    sounds/beebomb.wav \
    sounds/beesting.wav \
    sounds/stats.wav \
    data/AndroidManifest.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/mipmap-hdpi/ic_launcher.png \
    android/res/mipmap-mdpi/ic_launcher.png \
    android/res/mipmap-xhdpi/ic_launcher.png \
    android/res/mipmap-xxhdpi/ic_launcher.png \
    android/res/mipmap-xxxhdpi/ic_launcher.png \
    data/Terminus.png \
    ios/ios/apple-touch-icon-114x114-precomposed.png \
    ios/ios/apple-touch-icon-144x144-precomposed.png \
    ios/ios/apple-touch-icon-72x72-precomposed.png \
    ios/ios/apple-touch-icon-precomposed-57.png \
    ios/ios/Icon-57.png \
    ios/ios/Icon-57@2x.png \
    ios/ios/Icon-72.png \
    ios/ios/Icon-72@2x.png \
    ios/ios/Icon-Small-20.png \
    ios/ios/Icon-Small-20@2x.png \
    ios/ios/Icon-Small-29.png \
    ios/ios/Icon-Small-29@2x.png \
    ios/ios/Icon-Small-30.png \
    ios/ios/Icon-Small-30@2x.png \
    ios/ios/Icon-Small-50.png \
    ios/ios/Icon-Small-50@2x.png \
    ios/ios/iTunesArtwork.png \
    ios/ios/iTunesArtwork@2x.png \
    ios/sizes/Terminus-100.png \
    ios/sizes/Terminus-1024.png \
    ios/sizes/Terminus-114.png \
    ios/sizes/Terminus-144.png \
    ios/sizes/Terminus-20.png \
    ios/sizes/Terminus-29.png \
    ios/sizes/Terminus-30.png \
    ios/sizes/Terminus-320.png \
    ios/sizes/Terminus-40.png \
    ios/sizes/Terminus-48.png \
    ios/sizes/Terminus-50.png \
    ios/sizes/Terminus-512.png \
    ios/sizes/Terminus-57.png \
    ios/sizes/Terminus-58.png \
    ios/sizes/Terminus-60.png \
    ios/sizes/Terminus-64.png \
    ios/sizes/Terminus-640.png \
    ios/sizes/Terminus-72.png \
    ios/sizes/Terminus-96.png \
    data/terminus.ico \
    source/bullet/src/BulletCollision/premake4.lua \
    source/bullet/src/BulletDynamics/premake4.lua \
    source/bullet/src/BulletSoftBody/premake4.lua \
    source/bullet/src/LinearMath/premake4.lua \
    .gitignore \
    README.md \
    Terminus.pro.user \
    data/myapp.rc \
    source/bullet/src/BulletCollision/CMakeLists.txt \
    source/bullet/src/BulletDynamics/CMakeLists.txt \
    source/bullet/src/BulletSoftBody/CMakeLists.txt \
    source/bullet/src/LinearMath/CMakeLists.txt \
    source/bullet/src/CMakeLists.txt \
    source/bullet/BulletCollision/premake4.lua \
    source/bullet/BulletDynamics/premake4.lua \
    source/bullet/BulletSoftBody/premake4.lua \
    source/bullet/LinearMath/premake4.lua \
    source/bullet/BulletCollision/CMakeLists.txt \
    source/bullet/BulletDynamics/CMakeLists.txt \
    source/bullet/BulletSoftBody/CMakeLists.txt \
    source/bullet/LinearMath/CMakeLists.txt \
    source/bullet/CMakeLists.txt \
    source/qml/main.qml \
    source/qml/EnemyStatusBar.qml \
    source/qml/EnemyWagonStatus.qml \
    source/qml/PlayerStatusBar.qml \
    source/qml/PlayerWagonStatus.qml \
    source/qml/Reticle.qml \
    source/qml/StatusBarContainer.qml \
    source/qml/WagonActionArea.qml \
    source/qml/WagonSwitchArea.qml \
    source/qml/Game.qml \
    source/qml/MainMenu.qml \
    source/qml/MenuButton.qml \
    source/qml/MultiplayerMenu.qml \
    source/qml/Victory.qml \
    source/qml/Defeat.qml \
    source/qml/Headline.qml

RC_FILE = data/myapp.rc

INCLUDEPATH += \
    source/bullet \
    source

unix:LIBS += -ldl

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = source/qml

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    source/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
    source/bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
    source/bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
    source/bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
    source/bullet/BulletCollision/BroadphaseCollision/btDbvt.h \
    source/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
    source/bullet/BulletCollision/BroadphaseCollision/btDispatcher.h \
    source/bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h \
    source/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
    source/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
    source/bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
    source/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
    source/bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionObject.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionWorldImporter.h \
    source/bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
    source/bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btGhostObject.h \
    source/bullet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.h \
    source/bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
    source/bullet/BulletCollision/CollisionDispatch/btManifoldResult.h \
    source/bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
    source/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
    source/bullet/BulletCollision/CollisionDispatch/btUnionFind.h \
    source/bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
    source/bullet/BulletCollision/CollisionShapes/btBox2dShape.h \
    source/bullet/BulletCollision/CollisionShapes/btBoxShape.h \
    source/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
    source/bullet/BulletCollision/CollisionShapes/btCapsuleShape.h \
    source/bullet/BulletCollision/CollisionShapes/btCollisionMargin.h \
    source/bullet/BulletCollision/CollisionShapes/btCollisionShape.h \
    source/bullet/BulletCollision/CollisionShapes/btCompoundShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConcaveShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConeShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvex2dShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexHullShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexInternalShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexShape.h \
    source/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
    source/bullet/BulletCollision/CollisionShapes/btCylinderShape.h \
    source/bullet/BulletCollision/CollisionShapes/btEmptyShape.h \
    source/bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
    source/bullet/BulletCollision/CollisionShapes/btMaterial.h \
    source/bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
    source/bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
    source/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h \
    source/bullet/BulletCollision/CollisionShapes/btOptimizedBvh.h \
    source/bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
    source/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
    source/bullet/BulletCollision/CollisionShapes/btShapeHull.h \
    source/bullet/BulletCollision/CollisionShapes/btSphereShape.h \
    source/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
    source/bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
    source/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleBuffer.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleCallback.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleMesh.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
    source/bullet/BulletCollision/CollisionShapes/btTriangleShape.h \
    source/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h \
    source/bullet/BulletCollision/Gimpact/btBoxCollision.h \
    source/bullet/BulletCollision/Gimpact/btClipPolygon.h \
    source/bullet/BulletCollision/Gimpact/btCompoundFromGimpact.h \
    source/bullet/BulletCollision/Gimpact/btContactProcessing.h \
    source/bullet/BulletCollision/Gimpact/btGenericPoolAllocator.h \
    source/bullet/BulletCollision/Gimpact/btGeometryOperations.h \
    source/bullet/BulletCollision/Gimpact/btGImpactBvh.h \
    source/bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
    source/bullet/BulletCollision/Gimpact/btGImpactMassUtil.h \
    source/bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
    source/bullet/BulletCollision/Gimpact/btGImpactShape.h \
    source/bullet/BulletCollision/Gimpact/btQuantization.h \
    source/bullet/BulletCollision/Gimpact/btTriangleShapeEx.h \
    source/bullet/BulletCollision/Gimpact/gim_array.h \
    source/bullet/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
    source/bullet/BulletCollision/Gimpact/gim_bitset.h \
    source/bullet/BulletCollision/Gimpact/gim_box_collision.h \
    source/bullet/BulletCollision/Gimpact/gim_box_set.h \
    source/bullet/BulletCollision/Gimpact/gim_clip_polygon.h \
    source/bullet/BulletCollision/Gimpact/gim_contact.h \
    source/bullet/BulletCollision/Gimpact/gim_geom_types.h \
    source/bullet/BulletCollision/Gimpact/gim_geometry.h \
    source/bullet/BulletCollision/Gimpact/gim_hash_table.h \
    source/bullet/BulletCollision/Gimpact/gim_linear_math.h \
    source/bullet/BulletCollision/Gimpact/gim_math.h \
    source/bullet/BulletCollision/Gimpact/gim_memory.h \
    source/bullet/BulletCollision/Gimpact/gim_radixsort.h \
    source/bullet/BulletCollision/Gimpact/gim_tri_collision.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btComputeGjkEpaPenetration.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkCollisionDescription.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa3.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btMprPenetration.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
    source/bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h \
    source/bullet/BulletDynamics/Character/btCharacterControllerInterface.h \
    source/bullet/BulletDynamics/Character/btKinematicCharacterController.h \
    source/bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
    source/bullet/BulletDynamics/ConstraintSolver/btContactConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
    source/bullet/BulletDynamics/ConstraintSolver/btFixedConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btGearConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
    source/bullet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.h \
    source/bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
    source/bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btSolverBody.h \
    source/bullet/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
    source/bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
    source/bullet/BulletDynamics/Dynamics/btActionInterface.h \
    source/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
    source/bullet/BulletDynamics/Dynamics/btDynamicsWorld.h \
    source/bullet/BulletDynamics/Dynamics/btRigidBody.h \
    source/bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBody.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyConstraint.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyLink.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyPoint2Point.h \
    source/bullet/BulletDynamics/Featherstone/btMultiBodySolverConstraint.h \
    source/bullet/BulletDynamics/MLCPSolvers/btDantzigLCP.h \
    source/bullet/BulletDynamics/MLCPSolvers/btDantzigSolver.h \
    source/bullet/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.h \
    source/bullet/BulletDynamics/MLCPSolvers/btLemkeSolver.h \
    source/bullet/BulletDynamics/MLCPSolvers/btMLCPSolver.h \
    source/bullet/BulletDynamics/MLCPSolvers/btMLCPSolverInterface.h \
    source/bullet/BulletDynamics/MLCPSolvers/btPATHSolver.h \
    source/bullet/BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h \
    source/bullet/BulletDynamics/Vehicle/btRaycastVehicle.h \
    source/bullet/BulletDynamics/Vehicle/btVehicleRaycaster.h \
    source/bullet/BulletDynamics/Vehicle/btWheelInfo.h \
    source/bullet/BulletSoftBody/btDefaultSoftBodySolver.h \
    source/bullet/BulletSoftBody/btSoftBody.h \
    source/bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h \
    source/bullet/BulletSoftBody/btSoftBodyData.h \
    source/bullet/BulletSoftBody/btSoftBodyHelpers.h \
    source/bullet/BulletSoftBody/btSoftBodyInternals.h \
    source/bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h \
    source/bullet/BulletSoftBody/btSoftBodySolvers.h \
    source/bullet/BulletSoftBody/btSoftBodySolverVertexBuffer.h \
    source/bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.h \
    source/bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h \
    source/bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.h \
    source/bullet/BulletSoftBody/btSparseSDF.h \
    source/bullet/clew/clew.h \
    source/bullet/LinearMath/btAabbUtil2.h \
    source/bullet/LinearMath/btAlignedAllocator.h \
    source/bullet/LinearMath/btAlignedObjectArray.h \
    source/bullet/LinearMath/btConvexHull.h \
    source/bullet/LinearMath/btConvexHullComputer.h \
    source/bullet/LinearMath/btCpuFeatureUtility.h \
    source/bullet/LinearMath/btDefaultMotionState.h \
    source/bullet/LinearMath/btGeometryUtil.h \
    source/bullet/LinearMath/btGrahamScan2dConvexHull.h \
    source/bullet/LinearMath/btHashMap.h \
    source/bullet/LinearMath/btIDebugDraw.h \
    source/bullet/LinearMath/btList.h \
    source/bullet/LinearMath/btMatrix3x3.h \
    source/bullet/LinearMath/btMatrixX.h \
    source/bullet/LinearMath/btMinMax.h \
    source/bullet/LinearMath/btMotionState.h \
    source/bullet/LinearMath/btPolarDecomposition.h \
    source/bullet/LinearMath/btPoolAllocator.h \
    source/bullet/LinearMath/btQuadWord.h \
    source/bullet/LinearMath/btQuaternion.h \
    source/bullet/LinearMath/btQuickprof.h \
    source/bullet/LinearMath/btRandom.h \
    source/bullet/LinearMath/btScalar.h \
    source/bullet/LinearMath/btSerializer.h \
    source/bullet/LinearMath/btStackAlloc.h \
    source/bullet/LinearMath/btTransform.h \
    source/bullet/LinearMath/btTransformUtil.h \
    source/bullet/LinearMath/btVector3.h \
    source/bullet/btBulletCollisionCommon.h \
    source/bullet/btBulletDynamicsCommon.h \
    source/network/commands/abstractcommand.h \
    source/network/commands/commands.h \
    source/network/commands/projectilefiredcommand.h \
    source/network/networkclient.h \
    source/network/networkconnection.h \
    source/network/networkendpoint.h \
    source/network/networkmanager.h \
    source/network/networkserver.h \
    source/player/abstractplayer.h \
    source/player/aiplayer.h \
    source/player/localplayer.h \
    source/player/remoteplayer.h \
    source/resources/geometry.h \
    source/resources/indextriple.h \
    source/resources/light.h \
    source/resources/lightmanager.h \
    source/resources/material.h \
    source/resources/program.h \
    source/resources/resourcemanager.h \
    source/resources/soundmanager.h \
    source/util/catmullromspline.h \
    source/util/mathutil.h \
    source/util/perlinnoise.h \
    source/util/polyline.h \
    source/util/timer.h \
    source/util/tostring.h \
    source/world/drawables/train/wagons/abstractwagon.h \
    source/world/drawables/train/wagons/enginewagon.h \
    source/world/drawables/train/wagons/repairwagon.h \
    source/world/drawables/train/wagons/weaponwagon.h \
    source/world/drawables/train/train.h \
    source/world/drawables/train/train.hpp \
    source/world/drawables/abstractgraphicsobject.h \
    source/world/drawables/level.h \
    source/world/drawables/projectile.h \
    source/world/drawables/skybox.h \
    source/world/drawables/terrain.h \
    source/world/drawables/track.h \
    source/world/physics/abstractphysicsobject.h \
    source/world/physics/bulletworld.h \
    source/world/physics/dynamicphysicsobject.h \
    source/world/physics/kinematicphysicsobject.h \
    source/world/camera.h \
    source/world/world.h \
    source/deferredactionhandler.h \
    source/eventhandler.h \
    source/game.h \
    source/network/commands/preparenewgamecommand.h \
    source/network/ipprovider.h

SOURCES += \
    source/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
    source/bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btGhostObject.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/CollisionDispatch/btUnionFind.cpp \
    source/bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
    source/bullet/BulletCollision/CollisionShapes/btBox2dShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btBoxShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btCompoundShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConeShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btCylinderShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btEmptyShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
    source/bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btShapeHull.cpp \
    source/bullet/BulletCollision/CollisionShapes/btSphereShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
    source/bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
    source/bullet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
    source/bullet/BulletCollision/Gimpact/btContactProcessing.cpp \
    source/bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
    source/bullet/BulletCollision/Gimpact/btGImpactBvh.cpp \
    source/bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
    source/bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
    source/bullet/BulletCollision/Gimpact/btGImpactShape.cpp \
    source/bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
    source/bullet/BulletCollision/Gimpact/gim_box_set.cpp \
    source/bullet/BulletCollision/Gimpact/gim_contact.cpp \
    source/bullet/BulletCollision/Gimpact/gim_memory.cpp \
    source/bullet/BulletCollision/Gimpact/gim_tri_collision.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
    source/bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
    source/bullet/BulletDynamics/Character/btKinematicCharacterController.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
    source/bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
    source/bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
    source/bullet/BulletDynamics/Dynamics/btRigidBody.cpp \
    source/bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBody.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp \
    source/bullet/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp \
    source/bullet/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp \
    source/bullet/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp \
    source/bullet/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp \
    source/bullet/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
    source/bullet/BulletDynamics/Vehicle/btWheelInfo.cpp \
    source/bullet/BulletSoftBody/btDefaultSoftBodySolver.cpp \
    source/bullet/BulletSoftBody/btSoftBody.cpp \
    source/bullet/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp \
    source/bullet/BulletSoftBody/btSoftBodyHelpers.cpp \
    source/bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp \
    source/bullet/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp \
    source/bullet/BulletSoftBody/btSoftRigidDynamicsWorld.cpp \
    source/bullet/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp \
    source/bullet/LinearMath/btAlignedAllocator.cpp \
    source/bullet/LinearMath/btConvexHull.cpp \
    source/bullet/LinearMath/btConvexHullComputer.cpp \
    source/bullet/LinearMath/btGeometryUtil.cpp \
    source/bullet/LinearMath/btPolarDecomposition.cpp \
    source/bullet/LinearMath/btQuickprof.cpp \
    source/bullet/LinearMath/btSerializer.cpp \
    source/bullet/LinearMath/btVector3.cpp \
    source/network/commands/abstractcommand.cpp \
    source/network/commands/projectilefiredcommand.cpp \
    source/network/networkclient.cpp \
    source/network/networkconnection.cpp \
    source/network/networkendpoint.cpp \
    source/network/networkmanager.cpp \
    source/network/networkserver.cpp \
    source/player/abstractplayer.cpp \
    source/player/aiplayer.cpp \
    source/player/localplayer.cpp \
    source/player/remoteplayer.cpp \
    source/resources/geometry.cpp \
    source/resources/indextriple.cpp \
    source/resources/light.cpp \
    source/resources/lightmanager.cpp \
    source/resources/material.cpp \
    source/resources/program.cpp \
    source/resources/resourcemanager.cpp \
    source/resources/soundmanager.cpp \
    source/util/catmullromspline.cpp \
    source/util/mathutil.cpp \
    source/util/perlinnoise.cpp \
    source/util/polyline.cpp \
    source/util/timer.cpp \
    source/world/drawables/train/wagons/abstractwagon.cpp \
    source/world/drawables/train/wagons/enginewagon.cpp \
    source/world/drawables/train/wagons/repairwagon.cpp \
    source/world/drawables/train/wagons/weaponwagon.cpp \
    source/world/drawables/train/train.cpp \
    source/world/drawables/abstractgraphicsobject.cpp \
    source/world/drawables/level.cpp \
    source/world/drawables/projectile.cpp \
    source/world/drawables/skybox.cpp \
    source/world/drawables/terrain.cpp \
    source/world/drawables/track.cpp \
    source/world/physics/abstractphysicsobject.cpp \
    source/world/physics/bulletworld.cpp \
    source/world/physics/dynamicphysicsobject.cpp \
    source/world/physics/kinematicphysicsobject.cpp \
    source/world/camera.cpp \
    source/world/world.cpp \
    source/deferredactionhandler.cpp \
    source/eventhandler.cpp \
    source/game.cpp \
    source/main.cpp \
    source/bullet/clew/clew.c \
    source/network/commands/preparenewgamecommand.cpp \
    source/network/ipprovider.cpp
