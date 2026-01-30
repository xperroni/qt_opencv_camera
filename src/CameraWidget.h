#pragma once

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QTimer>
#include <QMutex>
#include <opencv2/opencv.hpp>

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget() override;

    // Start camera (device index, default 0)
    bool startCamera(int deviceIndex = 0);

    // Stop camera
    void stopCamera();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void grabFrame();

private:
    QImage matToQImage(const cv::Mat &mat);

    QLabel *m_label;
    QTimer *m_timer;
    cv::VideoCapture m_cap;
    QMutex m_mutex;
    bool m_running = false;
};