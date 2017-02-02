/****************************************************************************
 *
 * This file is part of the ustk software.
 * Copyright (C) 2016 - 2017 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ustk with software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a ViSP Professional
 * Edition License.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, please contact
 * Inria at ustk@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Authors:
 * Marc Pouliquen
 *
 *****************************************************************************/

/**
* @file usResliceWorldViewer.cpp
* @brief Graphical main window containing 4 vtk views.
*/

#include <visp3/ustk_core/usImagePostScan3D.h>
#include <visp3/ustk_gui/usVTKConverter.h>
#include <visp3/ustk_gui/usResliceWorldViewer.h>

#ifdef USTK_HAVE_VTK_QT

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkResliceImageViewer.h>
#include <vtkResliceCursorLineRepresentation.h>
#include <vtkResliceCursorThickLineRepresentation.h>
#include <vtkResliceCursorWidget.h>
#include <vtkResliceCursorActor.h>
#include <vtkResliceCursorPolyDataAlgorithm.h>
#include <vtkResliceCursor.h>
#include <vtkMetaImageReader.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkImageData.h>
#include <vtkCommand.h>
#include <vtkPlaneSource.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageSlabReslice.h>
#include <vtkBoundedPlanePointPlacer.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkHandleRepresentation.h>
#include <vtkResliceImageViewerMeasurements.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkCamera.h>
#include <vtkRendererCollection.h>
#include <vtkMatrix4x4.h>
#include <vtkAbstractTransform.h>
#include <vtkImageActor.h>
#include <vtkMapper.h>
#include <vtkActor2D.h>
#include <vtkActor.h>

#include <vtkHomogeneousTransform.h>

#include <QDesktopWidget>
#include <QResizeEvent>

/**
* Constructor.
* @param imageFileName the mhd file to read.
*/
usResliceWorldViewer::usResliceWorldViewer(std::string imageFileName )
{
  this->setupUi();
  usImageIo::read(postScanImage,imageFileName);
  usVTKConverter::convert(postScanImage,vtkImage);

  int imageDims[3];
  double spacing[3];
  vtkImage->GetDimensions(imageDims);
  vtkImage->GetSpacing(spacing);


  plane1 = vtkPlane::New();
  //plane1->SetNormal(0,0,1);
  //plane1->SetOrigin(0,0,0);
  //plane1->SetOrigin(imageDims[0]*spacing[0]/2,0,0);
  //plane1->SetNormal(1,0,0);

  plane2 = vtkPlane::New();
  //plane2->SetNormal(0,0,1);
  plane2->SetOrigin(0,0,0);
  plane2->SetNormal(0,1,0);
  //plane2->SetOrigin(0,imageDims[1]*spacing[1]/2,0);

  plane3 = vtkPlane::New();
  plane3->SetNormal(0,0,1);
  plane3->SetOrigin(0,0,0);
  //plane3->SetOrigin(0,0,imageDims[2]*spacing[2]/2);

  //matrix representing plane 3
  vpHomogeneousMatrix matrix3;
  matrix3.eye();
  matrix3[2][3] = imageDims[2]*spacing[2]/2;
  vtkMatrix3 = vtkMatrix4x4::New();
  usVTKConverter::convert(matrix3,vtkMatrix3);

  //matrix representing plane 2
  vpHomogeneousMatrix matrix2;
  matrix2.eye();
  matrix2[1][3] = imageDims[1]*spacing[1]/2;
  matrix2[0][0] = 0;
  matrix2[0][1] = 1;
  matrix2[1][1] = 0;
  matrix2[1][2] = 1;
  matrix2[2][0] = 1;
  matrix2[2][2] = 0;

  vtkMatrix2 = vtkMatrix4x4::New();
  usVTKConverter::convert(matrix2,vtkMatrix2);

  //matrix representing plane 1
  vpHomogeneousMatrix matrix1;
  matrix1.eye();
  matrix1[0][3] = imageDims[0]*spacing[0]/2;
  matrix1[0][0] = 0;
  matrix1[0][2] = 1;
  matrix1[1][1] = 0;
  matrix1[1][0] = 1;
  matrix1[2][1] = 1;
  matrix1[2][2] = 0;
  vtkMatrix1 = vtkMatrix4x4::New();
  usVTKConverter::convert(matrix1,vtkMatrix1);

  //Test RT matrix
  vpHomogeneousMatrix matrix_RotX45_TransZHalfDim;
  //rotation
  vpThetaUVector r;         // By default initialized to zero
  r[0] = vpMath::rad(45); // Rotation around x set to 45 degres converted in radians
  r[1] = 0;            // Rotation around y set to 0 radians
  r[2] = 0;               // Rotation around z set to 0 radians
  //translation
  vpTranslationVector t;
  t[0] = imageDims[0]*spacing[0]/2;
  t[1] = imageDims[1]*spacing[1]/2;
  t[2] = imageDims[2]*spacing[2]/2; // half size in Z
  matrix_RotX45_TransZHalfDim.insert(t);
  matrix_RotX45_TransZHalfDim.insert(r);

  vtkMatrix4x4 * vtkMatrix_RotX45_TransZHalfDim = vtkMatrix4x4::New();
  usVTKConverter::convert(matrix_RotX45_TransZHalfDim,vtkMatrix_RotX45_TransZHalfDim);

  vtkMatrix_RotX45_TransZHalfDim->Print(std::cout);

  //try to find corresponding normal for slice plane
  //plane1->SetNormal(0,0.5,-0.5);
  //plane1->SetOrigin(imageDims[0]*spacing[0]/2,imageDims[1]*spacing[1]/2,imageDims[2]*spacing[2]/2);


  this->view1->setImageData(vtkImage);
  this->view1->setResliceMatrix(vtkMatrix_RotX45_TransZHalfDim,plane1);
  this->view1->init();

  std::cout << "plane of reslice view at init :" << std::endl;
  this->plane1->Print(std::cout);

  this->view2->setImageData(vtkImage);
  this->view2->setPlanes(plane1,plane2,plane3);
  this->view2->init();
  this->view1->m_callback->widget3D = view2;

  // Set up action signals and slots
  connect(this->resetButton, SIGNAL(pressed()), this, SLOT(ResetViews()));
  //connect(this->update3D, SIGNAL(pressed()), view2, SLOT(updatePlane1()));
  ResetViews();
}

/**
* Exit slot, to exit the QApplication.
*/
void usResliceWorldViewer::slotExit()
{
  qApp->exit();
}

/**
* Reset views slot : reset the planes positions at the middle of the volume.
*/
void usResliceWorldViewer::ResetViews()
{
  // Reset the reslice image views
  for (int i = 0; i < 3; i++)
  {
    /*riw[i]->Reset();
    riw[i]->SetResliceMode(1);
    vi[i]->GetRenderer()->ResetCamera();
    if(i==2)
      riw[i]->GetRenderer()->GetActiveCamera()->SetRoll(180);*/
  }
  /*view1->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
  view2->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
  view3->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();*/
  this->Render();
}


/**
* Render slot, to recompute all the views.
*/
void usResliceWorldViewer::Render()
{
  this->view1->GetRenderWindow()->Render();
  this->view2->GetRenderWindow()->Render();

  this->view1->update();
  this->view2->update();
}

/**
* Setup all the widgets in the window.
*/
void usResliceWorldViewer::setupUi() {
  this->setMinimumSize(640,480);
  QRect screenRect = QApplication::desktop()->screenGeometry();
  this->resize(screenRect.size());

  gridLayoutWidget = new QWidget(this);
  gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
  gridLayoutWidget->setGeometry(QRect(10, 10, screenRect.width() - 200, screenRect.height() - 40));
  gridLayout_2 = new QGridLayout(gridLayoutWidget);
  gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
  gridLayout_2->setContentsMargins(0, 0, 0, 0);
  view1 = new us2DSceneWidget(gridLayoutWidget);
  view1->setObjectName(QString::fromUtf8("view1"));

  gridLayout_2->addWidget(view1, 0, 0, 1, 1);

  view2 = new us3DSceneWidget(gridLayoutWidget);
  view2->setObjectName(QString::fromUtf8("view2"));

  gridLayout_2->addWidget(view2, 0, 1, 1, 1);

  resetButton = new QPushButton(this);
  resetButton->setObjectName(QString::fromUtf8("resetButton"));
  resetButton->setText(QString::fromUtf8("Reset views"));
  resetButton->setGeometry(QRect(screenRect.width() - 180, 30, 160, 31));

  update3D = new QPushButton(this);
  update3D->setObjectName(QString::fromUtf8("update3D"));
  update3D->setText(QString::fromUtf8("update3D"));
  update3D->setGeometry(QRect(screenRect.width() - 180, 130, 160, 31));
}

/**
* Get the resize event of the window, to re-comute size and positions of all widgets/layouts.
*/
void usResliceWorldViewer::resizeEvent(QResizeEvent* event)
{
  //Min size : 640*480
  if(event->size().width() >= 640 && event->size().height() >= 480) {
    QMainWindow::resizeEvent(event);
    gridLayoutWidget->setGeometry(QRect(10, 10, event->size().width() - 220, event->size().height() - 20));
    resetButton->setGeometry(QRect(event->size().width() - 180, 30, 160, 31));
    update3D->setGeometry(QRect(event->size().width() - 180, 130, 160, 31));
  }
}
#endif
