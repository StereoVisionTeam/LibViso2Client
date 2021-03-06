#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <functioncodes.h>
#include <errorcodes.h>
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void readData();

  void on_pushButtonSetPatternSize_clicked();

  void on_pushButtonAddCalibrationSample_clicked();

  void on_pushButtonGetImage_clicked();

  void on_pushButtonOpenVideo_clicked();

  void on_pushButtonCalibrateCamera_clicked();

  void on_pushButtonAddImgToOdo_clicked();

  void on_pushButtonLoadCalibration_clicked();

  void on_pushButtonCreateVisoObject_clicked();

  void on_pushButtonChangeParameters_clicked();

  void on_pushButtonContinousStream_clicked();

  void on_pushButtonContinousStreamStop_clicked();

  void on_pushButtoResetPose_clicked();

  void on_pushButtonCalibrateHeight_clicked();

  void on_pushButtonCalibrateHeightGo_clicked();

private:
  void addPointTo3D(double x, double y, double z);
  QByteArray formatRequest(u_int8_t transactionID, u_int8_t functionCode);
  double *pose;
  Ui::MainWindow *ui;
  QTcpSocket *myTcpSocket ;

  double cameraHeight;
  bool heightCalibrationState;
  int numberOfHeightCalibrationSamples;
  double heightCalibrationStartPosition;
  double heightCalibrationEndPosition;
  double heightCalibrationUserDistance;
};

#endif // MAINWINDOW_H
