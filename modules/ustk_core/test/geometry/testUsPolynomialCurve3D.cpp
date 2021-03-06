/****************************************************************************
 *
 * This file is part of the UsNeedleDetection software.
 * Copyright (C) 2013 - 2016 by Inria. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License ("GPL") as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * See the file COPYING at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact the
 * authors at Alexandre.Krupa@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author:
 * Jason Chevrie
 *
 *****************************************************************************/

/*!
  \example testUsPolynomialCurve3D.cpp

  USTK usPolynomialCurve3D test

  This example tests all functions declared in the usPolynomialCurve3D class.
*/

#include <visp3/ustk_core/usPolynomialCurve3D.h>

#include <vector>

#include <visp3/core/vpColVector.h>
#include <visp3/core/vpHomogeneousMatrix.h>
#include <visp3/core/vpMatrix.h>

int main()
{
  std::cout << "Start testUsPolynomialCurve3D" << std::endl;
  usPolynomialCurve3D curveDefault;
  std::cout << "done: usPolynomialCurve3D default contructor" << std::endl;
  usPolynomialCurve3D curve(2);
  std::cout << "done: usPolynomialCurve3D contructor with inital order" << std::endl;
  usPolynomialCurve3D curve2(curve);
  std::cout << "done: usPolynomialCurve3D copy contructor" << std::endl;
  curveDefault = curve;
  std::cout << "done: usPolynomialCurve3D assignment operator" << std::endl;

  curve.setOrder(3);
  std::cout << "done: usPolynomialCurve3D::setOrder" << std::endl;
  curve.getOrder();
  std::cout << "done: usPolynomialCurve3D::getOrder" << std::endl;

  curve.setStartParameter(1);
  std::cout << "done: usPolynomialCurve3D::setStartParameter" << std::endl;
  curve.getStartParameter();
  std::cout << "done: usPolynomialCurve3D::getStartParameter" << std::endl;
  curve.setEndParameter(2);
  std::cout << "done: usPolynomialCurve3D::setEndParameter" << std::endl;
  curve.getEndParameter();
  std::cout << "done: usPolynomialCurve3D::getEndParameter" << std::endl;
  curve.setBoundaries(0, 1);
  std::cout << "done: usPolynomialCurve3D::setBoundaries" << std::endl;

  vpMatrix M(3, 4, 0.1);
  curve.setPolynomialCoefficients(M);
  std::cout << "done: usPolynomialCurve3D::setPolynomialCoefficients" << std::endl;
  vpMatrix coef = curve.getPolynomialCoefficients();
  std::cout << "done: usPolynomialCurve3D::getPolynomialCoefficients" << std::endl;

  curve.setParametricLength(0.5);
  std::cout << "done: usPolynomialCurve3D::setParametricLength" << std::endl;
  curve.getParametricLength();
  std::cout << "done: usPolynomialCurve3D::getParametricLength" << std::endl;
  curve.setLength(0.6);
  std::cout << "done: usPolynomialCurve3D::setLength" << std::endl;
  curve.getLength(100);
  std::cout << "done: usPolynomialCurve3D::getLength" << std::endl;

  vpColVector point = curve.getPoint(0.25);
  std::cout << "done: usPolynomialCurve3D::getPoint" << std::endl;

  vpColVector params(5, 0);
  for (int i = 0; i < 5; i++)
    params[i] = 0.1 * i;
  vpMatrix points = curve.getPoints(params);
  std::cout << "done: usPolynomialCurve3D::getPoints" << std::endl;

  point = curve.getStartPoint();
  std::cout << "done: usPolynomialCurve3D::getStartPoint" << std::endl;
  point = curve.getEndPoint();
  std::cout << "done: usPolynomialCurve3D::getEndPoint" << std::endl;
  vpColVector tangent = curve.getTangent(0.25);
  std::cout << "done: usPolynomialCurve3D::getTangent" << std::endl;
  tangent = curve.getStartTangent();
  std::cout << "done: usPolynomialCurve3D::getStartTangent" << std::endl;
  tangent = curve.getEndTangent();
  std::cout << "done: usPolynomialCurve3D::getEndTangent" << std::endl;
  vpColVector deriv = curve.getDerivative(0.25, 2);
  std::cout << "done: usPolynomialCurve3D::.getDerivative" << std::endl;

  std::vector<vpColVector> points1(3, vpColVector(3, 0));
  points1.at(1)[0] = 1;
  points1.at(2)[1] = 1;
  std::vector<double> params1(3, 0);
  for (int i = 0; i < 3; i++)
    params1[i] = i;

  std::vector<vpColVector> points2(4, vpColVector(3, 0));
  points2.at(1)[0] = 1;
  points2.at(2)[1] = 2;
  points2.at(3)[2] = 1;
  std::vector<double> params2(4, 0);
  for (int i = 0; i < 4; i++)
    params2[i] = i;

  std::vector<vpColVector> points3(10, vpColVector(3, 0));
  points3.at(1)[0] = 1;
  points3.at(2)[1] = 2;
  points3.at(3)[2] = 3;
  points3.at(4)[1] = 1;
  points3.at(5)[1] = 2;
  points3.at(6)[2] = 3;
  points3.at(7)[0] = 1;
  points3.at(8)[0] = 2;
  points3.at(9)[0] = 2;
  points3.at(7)[1] = 2;
  points3.at(8)[1] = 2;
  points3.at(9)[2] = 2;
  std::vector<double> params3(10, 0);
  for (int i = 0; i < 10; i++)
    params3[i] = i;
  std::vector<double> weights3(10);
  for (int i = 0; i < 10; i++)
    weights3[i] = 1 + 0.2 * (i % 3 - 1);
  vpMatrix point3M(3, 10);
  for (int i = 0; i < 10; i++)
    point3M.insert(points3.at(i), 0, i);
  vpColVector params3V(10);
  for (int i = 0; i < 10; i++)
    params3V[i] = params3.at(i);
  vpColVector weights3V(10);
  for (int i = 0; i < 10; i++)
    weights3V[i] = weights3.at(i);

  curve.defineFromPoints(points1, params1, 4);
  std::cout << "done: case 1 usPolynomialCurve3D::defineFromPoints(const std::vector<vpColVector> &points, const "
               "std::vector<double> &param, unsigned int order)"
            << std::endl;
  curve.defineFromPoints(points2, params2, 4);
  std::cout << "done: case 2 usPolynomialCurve3D::defineFromPoints(const std::vector<vpColVector> &points, const "
               "std::vector<double> &param, unsigned int order)"
            << std::endl;
  curve.defineFromPoints(point3M, params3V, 3);
  std::cout << "done: case 3 usPolynomialCurve3D::defineFromPoints(const std::vector<vpColVector> &points, const "
               "std::vector<double> &param, unsigned int order)"
            << std::endl;

  curve.defineFromPoints(points3, params3, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromPoints(const vpMatrix points, const vpColVector &param, unsigned "
               "int order)"
            << std::endl;
  curve.defineFromPointsAuto(points3, 3);
  std::cout
      << "done: usPolynomialCurve3D::defineFromPointsAuto(const std::vector<vpColVector> &points, unsigned int order)"
      << std::endl;
  curve.defineFromPointsAuto(point3M, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromPointsAuto(const vpMatrix &points, unsigned int order)"
            << std::endl;

  vpColVector direction(3, 0);
  direction[0] = 1;
  curve.defineFromPointsAuto(points3, direction, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromPointsAuto(const std::vector<vpColVector> &points, const "
               "vpColVector &direction, unsigned int order)"
            << std::endl;
  curve.defineFromPointsAuto(point3M, direction, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromPointsAuto(const vpMatrix &points, const vpColVector &direction, "
               "unsigned int order)"
            << std::endl;
  curve.defineFromWeightedPoints(points3, params3, weights3, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPoints(const std::vector<vpColVector> &points, const "
               "std::vector<double> &param, const std::vector<double> &weights, unsigned int order)"
            << std::endl;
  curve.defineFromWeightedPoints(point3M, params3V, weights3V, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPoints(const vpMatrix &points, const vpColVector &param, "
               "const vpColVector &weights, unsigned int order)"
            << std::endl;

  curve.defineFromWeightedPointsAuto(points3, weights3, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPointsAuto(const std::vector<vpColVector> &points, const "
               "std::vector<double> &weights, unsigned int order)"
            << std::endl;
  curve.defineFromWeightedPointsAuto(point3M, weights3V, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPointsAuto(const vpMatrix &points, const vpColVector "
               "&weights, unsigned int order)"
            << std::endl;
  curve.defineFromWeightedPointsAuto(points3, weights3, direction, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPointsAuto(const std::vector<vpColVector> &points, const "
               "std::vector<double> &weights, const vpColVector &direction, unsigned int order)"
            << std::endl;
  curve.defineFromWeightedPointsAuto(point3M, weights3V, direction, 3);
  std::cout << "done: usPolynomialCurve3D::defineFromWeightedPointsAuto(const vpMatrix &points, const vpColVector "
               "&weights, const vpColVector &direction, unsigned int order)"
            << std::endl;

  curve.getCurvature(0.25);
  std::cout << "done: usPolynomialCurve3D::getCurvature" << std::endl;
  curve.getMeanAxisDeviation(100);
  std::cout << "done: usPolynomialCurve3D::getMeanAxisDeviation" << std::endl;

  vpMatrix CP = curve.getControlPoints();
  std::cout << "done: usPolynomialCurve3D::getControlPoints" << std::endl;
  curve.setControlPoints(CP);
  std::cout << "done: usPolynomialCurve3D::setControlPoints(const vpMatrix&)" << std::endl;
  curve.setControlPoints(&CP.data);
  std::cout << "done: usPolynomialCurve3D::setControlPoints(double**)" << std::endl;
  curve.getRenderingPoints();
  std::cout << "done: usPolynomialCurve3D::getRenderingPoints" << std::endl;

  curve2 = curve.getSubPolynomialCurve(0.1, 0.2);
  std::cout << "done: usPolynomialCurve3D::getSubPolynomialCurve" << std::endl;
  curve2 = curve.getNewOrderPolynomialCurve(5);
  std::cout << "done: case 1 usPolynomialCurve3D::getNewOrderPolynomialCurve" << std::endl;
  curve2 = curve.getNewOrderPolynomialCurve(1);
  std::cout << "done: case 2 usPolynomialCurve3D::getNewOrderPolynomialCurve" << std::endl;

  curve.changeCoefficientsToFitBoundaries(2, 3);
  std::cout << "done: usPolynomialCurve3D::changeCoefficientsToFitBoundaries" << std::endl;
  curve.reverse();
  std::cout << "done: usPolynomialCurve3D::reverse" << std::endl;
  curve.changeCoefficientsToFitMetricLength();
  std::cout << "done: usPolynomialCurve3D::changeCoefficientsToFitMetricLength" << std::endl;

  curve.move(vpHomogeneousMatrix(0.5, -0.25, 0.1, 1, -1, 0.2));
  std::cout << "done: usPolynomialCurve3D::move(const vpHomogeneousMatrix&)" << std::endl;
  curve.move(0.5, -0.25, 0.1, 1, -1, 0.2);
  std::cout << "done: usPolynomialCurve3D::move(double,double,double,double,double,double)" << std::endl;
  curve.scale(0.2);
  std::cout << "done: usPolynomialCurve3D::scale" << std::endl;

  return 0;
}
