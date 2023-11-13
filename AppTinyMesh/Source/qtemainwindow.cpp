#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"
#include "primitives.h"
#include <time.h>

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

    meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->sceneImplicit, SIGNAL(clicked()), this, SLOT(SceneExample()));
    connect(uiw->erosion, SIGNAL(clicked()), this, SLOT(ErosionExample()));
    connect(uiw->bezier, SIGNAL(clicked()), this, SLOT(BezierExample()));
    connect(uiw->revolution, SIGNAL(clicked()), this, SLOT(RevolutionExample()));
    connect(uiw->twist, SIGNAL(clicked()), this, SLOT(TwistExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));


	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
	connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
    connect(meshWidget, SIGNAL(_signalErosion(const Ray&)), this, SLOT(editingErosion(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
    std::cout<<"oui"<<std::endl;
}

void MainWindow::editingSceneRight(const Ray&)
{
    std::cout<<"non"<<std::endl;
}

void MainWindow::editingErosion(const Ray& r)
{
    double v;
//    std::cout<<tree.root->Value(Vector(0., 0., 5.));
    std::cout<<"lalala"<<std::endl;
    if (tree->root != nullptr)
    {
        std::cout<<"lilili"<<std::endl;
        if (tree->Intersect(r, v))
        {
            count++;
            Sphere* s = new Sphere(r.Origin()+v*r.Direction(), 0.1);
            sphere.push_back(s);
            if (count==8)
            {
                count = 0;

                Node* globalShape = tree->root;
                Node* unionSphere = new Union(new Union(new Union(sphere[0], sphere[1]), new Union(sphere[2], sphere[3])), new Union(new Union(sphere[4], sphere[5]), new Union(sphere[6], sphere[7])));
                globalShape = new DifferenceSmooth(globalShape, unionSphere);
                tree = new Tree(globalShape);
                Mesh implicitMesh;
                tree->root->Polygonize(63, implicitMesh, Box(2.0));

                std::vector<Color> cols;
                cols.resize(implicitMesh.Vertexes());
                for (size_t i = 0; i < cols.size(); i++)
                  cols[i] = Color(0.8, 0.8, 0.8);

                meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
                UpdateGeometry();

                sphere.clear();
            }
        }
//        Sphere implicit1(r.Origin()+v*r.Direction(), 0.2);
//        DifferenceSmooth implicit(tree->root, &implicit1);
    }
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));

	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
  AnalyticScalarField implicit;
  Mesh implicitMesh;
  implicit.Polygonize(310, implicitMesh, Box(2.0));

  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::BezierExample()
{
    // Création d'un ensemble de points de contrôle pour un patch 2x2 (n = 1, m = 1)
    std::vector<std::vector<Vector>> controlPoints{
        {Vector(0.0, 0.0, 0.0), Vector(1.0, 0.0, 2.0), Vector(2.0, 0.0, 0.0), Vector(3.0, 0.0, 2.0)},
        {Vector(0.0, 1.0, 0.0), Vector(1.0, 1.0, 2.0), Vector(2.0, 1.0, 0.0), Vector(3.0, 1.0, 2.0)},
        {Vector(0.0, 2.0, 0.0), Vector(1.0, 2.0, 2.0), Vector(2.0, 2.0, 0.0), Vector(3.0, 2.0, 2.0)},
        {Vector(0.0, 3.0, 0.0), Vector(1.0, 3.0, 2.0), Vector(2.0, 3.0, 0.0), Vector(3.0, 3.0, 2.0)}
    };

    // Création d'une instance de la classe Bezier
    Bezier bezier(3, 3, controlPoints);

    // Génération d'un maillage avec une certaine résolution
    Mesh mesh = bezier.Polygonize(100);

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::RevolutionExample()
{
    // Création d'un ensemble de points de contrôle pour un patch 2x2 (n = 1, m = 1)
    std::vector<Vector> controlPoints{
        Vector(0.3, 0.0, 0.0), Vector(1.1, 0.0, 1.0), Vector(0.2, 0.0, 1.6), Vector(0.6, 0.0, 2.0)
    };
    Vector axis(0.0, 0.0, 1.0);

    // Création d'une instance de la classe Bezier
    Revolution revolution(controlPoints, axis);

    // Génération d'un maillage avec une certaine résolution
    Mesh mesh = revolution.Polygonize(100);

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::TwistExample()
{
    // Création d'un ensemble de points de contrôle pour un patch 2x2 (n = 1, m = 1)
    std::vector<Vector> controlPoints{
        Vector(0.3, 0.0, 0.0), Vector(1.1, 0.0, 1.0), Vector(0.2, 0.0, 1.6), Vector(0.6, 0.0, 2.0)
    };
    Vector axis(0.0, 0.0, 1.0);

    // Création d'une instance de la classe Bezier
    Revolution revolution(controlPoints, axis);

    // Génération d'un maillage avec une certaine résolution
    Mesh mesh = revolution.Polygonize(100);

    Twist twist(M_PI/5.);
    twist.Warp(mesh);

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::ErosionExample()
{
    const long max_rand = 1000000L;
    double v = 0.0;

    Ray r(Vector(0.0), Vector(0.0));
    Node* globalShape = new Sphere(Vector(0.0), 1.5);
    tree = new Tree(globalShape);
    srandom(time(NULL));

    sphere.clear();
    count = 0;

    for(int i = 0; i<192; i++)
    {
        double d1, d2;
        d1 = 3. * (random() % max_rand) / max_rand - 1.5;
        d2 = 3. * (random() % max_rand) / max_rand - 1.5;
        std::cout<<d1<<" ; "<<d2<<std::endl;

        r = Ray(Vector(0, -5.0, 0), Vector(d1/5.0, 1.0, d2/5.0));
        if (tree->Intersect(r, v))
        {
            std::cout<<r.Origin()<<" "<<v<<" "<<r.Direction()<<std::endl;
            sphere.push_back(new Sphere(r.Origin()+v*r.Direction(), 0.1));


            count++;
            std::cout<<"count "<<count<<std::endl;
            if (count == 16)
            {
                Node* unionSphere = new Union(new Union(new Union(new Union(sphere[0], sphere[1]), new Union(sphere[2], sphere[3])), new Union(new Union(sphere[4], sphere[5]), new Union(sphere[6], sphere[7]))),
                                                new Union(new Union(new Union(sphere[8], sphere[9]), new Union(sphere[10], sphere[11])), new Union(new Union(sphere[12], sphere[13]), new Union(sphere[14], sphere[15]))));
                globalShape = new DifferenceSmooth(globalShape, unionSphere);
                tree = new Tree(globalShape);
                sphere.clear();
                count = 0;
            }
        }
    }

    Mesh implicitMesh;
    std::cout<<"là ?"<<std::endl;
    tree->root->Polygonize(63, implicitMesh, Box(1.5));
    std::cout<<"là !!!!"<<std::endl;

    std::vector<Color> cols;
    cols.resize(implicitMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
    UpdateGeometry();
}

void MainWindow::SceneExample()
{
      Ray r(Vector(.0, .0, 5.0), Vector(.0, .0, -1.0));

//      Node* globalShape = new Sphere(Vector(.0, .0, .0), 0.5);
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(.0, 1.0, .0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(1.0, .0, .0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(1.0, 1.0, .0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(.0, .0, 1.0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(.0, 1.0, 1.0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(1.0, .0, 1.0), 0.5));
//      globalShape = new UnionSmooth(globalShape, new Sphere(Vector(1.0, 1.0, 1.0), 0.5));

      Node* globalShape = new UnionSmooth(new UnionSmooth(new UnionSmooth(new Sphere(Vector(.0, .0, .0), 0.5), new Sphere(Vector(.0, 1.0, .0), 0.5)),
                                                          new UnionSmooth(new Sphere(Vector(1.0, .0, .0), 0.5), new Sphere(Vector(1.0, 1.0, .0), 0.5))),
                                          new UnionSmooth(new UnionSmooth(new Sphere(Vector(.0, .0, 1.0), 0.5), new Sphere(Vector(.0, 1.0, 1.0), 0.5)),
                                                          new UnionSmooth(new Sphere(Vector(1.0, .0, 1.0), 0.5), new Sphere(Vector(1.0, 1.0, 1.0), 0.5))));
      Node* union1 = new Union(new Capsule(Vector(-1., -1., -.5), Vector(-1., -1., 1.5), 0.2), new SquareBox(Vector(-1.1, -1.1, .0), Vector(-.7, -.7, .5)));
      globalShape = new Union (globalShape, union1);
      double v;
      tree = new Tree(globalShape);

      for (int i = 0; i<2; i++)
      {
          r = Ray(Vector(1.05, 1.05, 5.0), Vector(.0, .0, -1.0));
          if (tree->Intersect(r, v))
          {
              globalShape = new DifferenceSmooth(globalShape, new Sphere(r.Origin()+v*r.Direction(), 0.2));
              tree = new Tree(globalShape);
          }

          r = Ray(Vector(1.25, 1.05, 5.0), Vector(.0, .0, -1.0));
          if (tree->Intersect(r, v))
          {
              globalShape = new DifferenceSmooth(globalShape, new Sphere(r.Origin()+v*r.Direction(), 0.2));
              tree = new Tree(globalShape);
          }

          r = Ray(Vector(0.95, 1.05, 5.0), Vector(.0, .0, -1.0));
          if (tree->Intersect(r, v))
          {
              globalShape = new DifferenceSmooth(globalShape, new Sphere(r.Origin()+v*r.Direction(), 0.2));
              tree = new Tree(globalShape);
          }

          r = Ray(Vector(0.9, 0.9, 5.0), Vector(.0, .0, -1.0));
          if (tree->Intersect(r, v))
          {
              globalShape = new DifferenceSmooth(globalShape, new Sphere(r.Origin()+v*r.Direction(), 0.2));
              tree = new Tree(globalShape);
          }
    }

//      r = Ray(Vector(1.05, 1.05, 5.0), Vector(.0, .0, -1.0));
//      if (tree.Intersect(r, v))
//      {
//          std::cout<<"lilili"<<std::endl;
//          implicit115 = Sphere(r.Origin()+v*r.Direction(), 0.2);
//          std::cout<<"là"<<std::endl;
//      }
//      globalShape = new DifferenceSmooth(globalShape, &implicit115);
//      tree = Tree(globalShape);

//      if (tree.Intersect(r, v))
//      {
//          std::cout<<"lilili"<<std::endl;
//          implicit111 = Sphere(r.Origin()+v*r.Direction(), 0.4);
//          std::cout<<"là"<<std::endl;
//      }
//      globalShape = new DifferenceSmooth(globalShape, &implicit111);
//      tree = Tree(globalShape);

//      editingErosion(r);
      Mesh implicitMesh;
      globalShape->Polygonize(63, implicitMesh, Box(2.0));

      std::vector<Color> cols;
      cols.resize(implicitMesh.Vertexes());
      for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

      meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
      UpdateGeometry();
}

void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
	meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}
