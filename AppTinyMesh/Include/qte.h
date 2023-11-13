#ifndef __Qte__
#define __Qte__

#include <QtWidgets/qmainwindow.h>
#include "realtime.h"
#include "meshcolor.h"
#include "primitives.h"
#include "bezier.h"
#include <vector>

QT_BEGIN_NAMESPACE
	namespace Ui { class Assets; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::Assets* uiw;           //!< Interface

  MeshWidget* meshWidget;   //!< Viewer
  MeshColor meshColor;		//!< Mesh.
  int count = 0;
  std::vector<Node*> sphere;
  Tree* tree;

public:
  MainWindow();
  ~MainWindow();
  void CreateActions();
  void UpdateGeometry();

public slots:
  void editingSceneLeft(const Ray&);
  void editingSceneRight(const Ray&);
  void editingErosion(const Ray&);
  void BoxMeshExample();
  void SphereImplicitExample();
  void SceneExample();
  void ErosionExample();
  void BezierExample();
  void RevolutionExample();
  void TwistExample();
  void ResetCamera();
  void UpdateMaterial();
};

#endif
