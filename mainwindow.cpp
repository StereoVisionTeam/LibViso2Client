#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  pose = new double[12];

  ui->setupUi(this);
  myTcpSocket = new QTcpSocket(this);
  myTcpSocket->connectToHost("127.0.0.1",2000);
  if(myTcpSocket->waitForConnected(1000))
    ui->labelConnectionStatus->setText("Connected");
  else
    ui->labelConnectionStatus->setText("Not Connected");
  connect(this->myTcpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
  connect(this->ui->lineEditRange,SIGNAL(textChanged(QString)),ui->myGLWidget,SLOT(changeRange(QString)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::readData(){
  QByteArray myArray = myTcpSocket->readAll();
  if(myArray.size() < 6){
    qDebug()<<"Array to Small";
    return;
  }
  qDebug()<<"Start Byte" <<(quint8)myArray[0];
  qDebug()<<"Transaction ID" <<(quint8)myArray[1];
  quint8 functionCode = (quint8)myArray[2];
  qDebug()<<"Function Code" << functionCode;

  quint8 errorCode = (quint8)myArray[3];
  qDebug()<<"Error Code" <<errorCode;
  qDebug()<<"Data Length" <<(quint8)myArray[4];
  quint8 length = (quint8)myArray[4];

  if(myArray.size() != 6+length){
    qDebug()<<"Array has strange Size";
    return;
  }
  for(int i=5; i<5+length;i++)
    qDebug()<<"Data "<<i<< " "<<(quint8)myArray[i];
  qDebug()<<"Stop Byte" <<(quint8)myArray[myArray.size()-1];

  if(functionCode == ADD_IMG_TO_ODOMETRY ||
     functionCode == ADD_IMG_TO_ODOMETRY +0x80 ||
     functionCode == CONTINUOUS_STREAM){
      ui->textEditPose->clear();
      double *data = reinterpret_cast<double *>(myArray.data()+5);
      for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
          pose[j+4*i]=data[j+4*i];
          ui->textEditPose->insertPlainText(QString::number(pose[j+4*i])+QString("   "));
        }
          ui->textEditPose->append("");
        }
      addPointTo3D(pose[3], pose[7], pose[11]);
    }
  if((functionCode == CALIBRATE_CAMERA || functionCode == LOAD_CALIBRATION ) && length == 9*sizeof(double)){
      ui->textEditPose->clear();
      double *data = reinterpret_cast<double *>(myArray.data()+5);
      for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
          pose[j+3*i]=data[j+3*i];
          ui->textEditPose->insertPlainText(QString::number(pose[j+3*i])+QString("  "));
        }
          ui->textEditPose->append("");
        }
    }

  ui->labelErrorCode->setText(QString::number(errorCode));
}

void MainWindow::on_pushButtonSetPatternSize_clicked()
{
    QByteArray array;
    array.push_back(START_BYTE);
    array.push_back((char)0);
    array.push_back(SET_PATTERN_SIZE);
    array.push_back((char)2);
    quint8 x = ui->lineEditPatternSizeX->text().toInt();
    quint8 y = ui->lineEditPatternSizeY->text().toInt();

    array.push_back(x);
    array.push_back(y);
    array.push_back(STOP_BYTE);
    myTcpSocket->write(array);
}

void MainWindow::on_pushButtonAddCalibrationSample_clicked()
{
  QByteArray array = formatRequest(0,ADD_SAMPLE_TO_CALIBRATION);
  myTcpSocket->write(array);
}

void MainWindow::on_pushButtonGetImage_clicked()
{
  QByteArray array = formatRequest(0,GET_FRAME);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

void MainWindow::on_pushButtonOpenVideo_clicked()
{
  QByteArray array = formatRequest(0,OPEN_VIDEO);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

void MainWindow::on_pushButtonCalibrateCamera_clicked()
{
  QByteArray array = formatRequest(0,CALIBRATE_CAMERA);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

void MainWindow::on_pushButtonAddImgToOdo_clicked()
{
  QByteArray array = formatRequest(0,ADD_IMG_TO_ODOMETRY);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

void MainWindow::on_pushButtonLoadCalibration_clicked(){
  QByteArray array = formatRequest(0,LOAD_CALIBRATION);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

QByteArray MainWindow::formatRequest(u_int8_t transactionID, u_int8_t functionCode){
  QByteArray array;
  array.push_back(START_BYTE);
  array.push_back(transactionID);
  array.push_back(functionCode);
  array.push_back((char)0);
  array.push_back(STOP_BYTE);
  return array;
}
void MainWindow::on_pushButtonCreateVisoObject_clicked(){
  QByteArray array = formatRequest(0,CREATE_VISO_OBJECT);
  myTcpSocket->write(array);
  myTcpSocket->flush();
}

void MainWindow::on_pushButtonChangeParameters_clicked()
{
  double height = ui->lineEditHeight->text().toDouble();
  double pitch = ui->lineEditPitch->text().toDouble();
  QByteArray array;
  array.push_back(START_BYTE);
  array.push_back((char)0);
  array.push_back(CHANGE_PARAMETERS);
  quint8 dataSize = 2*sizeof(double);
  array.push_back(dataSize);

  for(int i=0;i<dataSize; i++)
    array.push_back(char(0));
  double *pointer;
  pointer = reinterpret_cast<double *>(array.data()+4);
  *pointer = height;
  pointer++;
  *pointer = pitch;
  quint8 y = ui->lineEditPatternSizeY->text().toInt();

  array.push_back(STOP_BYTE);
  myTcpSocket->write(array);
}
void MainWindow::on_pushButtonContinousStream_clicked(){
  QByteArray array;
  array.push_back(START_BYTE);
  array.push_back((char)0);
  array.push_back(CONTINUOUS_STREAM);
  array.push_back((char)1);
  array.push_back(TRUE);
  array.push_back(STOP_BYTE);
  myTcpSocket->write(array);
}
void MainWindow::on_pushButtonContinousStreamStop_clicked(){
  QByteArray array;
  array.push_back(START_BYTE);
  array.push_back((char)0);
  array.push_back(CONTINUOUS_STREAM);
  array.push_back((char)1);
  array.push_back((char)FALSE);
  array.push_back(STOP_BYTE);
  myTcpSocket->write(array);
}
void MainWindow::addPointTo3D(double x, double y, double z){
  QList<float> point;
  point.push_back(x);
  point.push_back(y);
  point.push_back(z);
  this->ui->myGLWidget->myPoints.push_back(point);
  this->ui->myGLWidget->update();
}

void MainWindow::on_pushButtoResetPose_clicked()
{
  QByteArray array = formatRequest(0,RESET_POSE);
  myTcpSocket->write(array);
  myTcpSocket->flush();
  ui->myGLWidget->myPoints.clear();
  ui->myGLWidget->update();
}
